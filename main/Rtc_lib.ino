#ifdef RTC_ENABLE

////public API
//void setEpoch(unsigned long epoch);
//unsigned long getEpoch();

////private API
//void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, 
//                   byte dayOfMonth, byte month, byte year);
//void readDS3231time(byte *second,byte *minute,byte *hour,byte *dayOfWeek,
//                    byte *dayOfMonth,byte *month,byte *year);
//byte decToBcd(byte val);
//byte bcdToDec(byte val);
//unsigned long GetEpochTime(const tmElements_t &tm);
//void GetDateTimeFromEpoch (tmElements_t* tm, uint32_t unix);


#define LEAP_YEAR(Y)     ( ((1970+(Y))>0) && !((1970+(Y))%4) && ( ((1970+(Y))%100) || !((1970+(Y))%400) ) ) // leap year calculator expects year argument as years offset from 1970
#define LEAP_YEARS(year)             ((((year) % 4 == 0) && ((year) % 100 != 0)) || ((year) % 400 == 0))
 
//static  const uint8_t monthDays[]={31,28,31,30,31,30,31,31,30,31,30,31}; // API starts months from 1, this array starts from 0
 /* Days in a month */
static uint8_t RTC_Months[2][12] = {
  {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}, /* Not leap year */
  {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}  /* Leap year */
};




void setEpoch(unsigned long epoch){    //set epoch value to DS3231
    tmElements_t TM;
    
    GetDateTimeFromEpoch(&TM, epoch);
    
    setDS3231time(TM.Second , TM.Minute ,TM.Hour ,TM.Wday,
                  TM.DayOfmonth, TM.Month, TM.Year);
}

unsigned long getEpoch(){    //get epoch value from DS3231
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  
   //eemprom content update
   tmElements_t te;  //Time elements structure
   te.Second     = second;
   te.Minute     = minute;
   te.Hour       = hour; //11 pm
   te.DayOfmonth = dayOfMonth;
   te.Month      = month;
   te.Year       = 2000 + year - 1970; //Y2K, in seconds = 946684800UL 
   return GetEpochTime(te);   
}

//----------------------RTC read/write---------
//-----------------RTC_set_get_func---------------------
/*
 * setDS3231time: 
 * set time to rtc(DS3231) byte by byte
 */
void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
dayOfMonth, byte month, byte year)
{
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}

/*
 * readDS3231time: 
 * read time from rtc(DS3231) byte by byte
 */
void readDS3231time(byte *second,
byte *minute,
byte *hour,
byte *dayOfWeek,
byte *dayOfMonth,
byte *month,
byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}


//---------------------------- 

byte decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}


//-------------------Conversion of RTC------------------------
//--------------------date format to Unix format--------------------

unsigned long GetEpochTime(const tmElements_t &tm){   
// assemble time elements into time_t 
// note year argument is offset from 1970 (see macros in time.h to convert to other formats)
// previous version used full four digit year (or digits since 2000),i.e. 2009 was 2009 or 9
  
  int i;
  uint32_t seconds;
  
//  uint32_t SECS_PER_HOUR = 3600;
//  uint32_t SECS_PER_DAY  = SECS_PER_HOUR*24;
//  uint32_t SECS_PER_MIN  = 60; 
   
  // seconds from 1970 till 1 jan 00:00:00 of the given year
  seconds= tm.Year*(SECS_PER_DAY * 365);
  for (i = 0; i < tm.Year; i++) {
    if (LEAP_YEAR(i)) {
      seconds += SECS_PER_DAY;   // add extra days for leap years
    }
  }
  
  // add days for this year, months start from 1
  for (i = 1; i < tm.Month; i++) {
    if ( (i == 2) && LEAP_YEAR(tm.Year)) { 
      seconds += SECS_PER_DAY * 29;
    } else {
      seconds += SECS_PER_DAY * monthDays[i-1];  //monthDay array starts from 0
    }
  }
  seconds+= (tm.DayOfmonth-1) * SECS_PER_DAY;
  seconds+= tm.Hour * SECS_PER_HOUR;
  seconds+= tm.Minute * SECS_PER_MIN;
  seconds+= tm.Second;
  return (unsigned long)seconds; 
} 

//-----------------------Unix format to date format --------------
 
void GetDateTimeFromEpoch (tmElements_t* tm, uint32_t unix) {
    uint16_t year;      
    tm->Second = unix % 60;  /* Get seconds from unix */
    unix /= 60;                 /* Go to minutes */
    tm->Minute = unix % 60;  /* Get minutes */
    unix /= 60;                 /* Go to hours */
    tm->Hour = unix % 24;    /* Get hours */
    unix /= 24;                 /* Go to days */
    tm->Wday = (unix + 3) % 7 + 1; /* Get week day, monday is first day */

    year = 1970;                /* Process year */
    while (1) {
        if (LEAP_YEARS(year)) {
            if (unix >= 366) {
                unix -= 366;
            } else {
                break;
            }
        } else if (unix >= 365) {
            unix -= 365;
        } else {
            break;
        }
        year++;
    }
    /* Get year in xx format */
    tm->Year = (uint8_t) (year - 2000);
    /* Get month */
    for (tm->Month = 0; tm->Month < 12; tm->Month++) {
        if (LEAP_YEARS(year)) {
            if (unix >= (uint32_t)RTC_Months[1][tm->Month]) {
                unix -= RTC_Months[1][tm->Month];
            } else {
                break;
            }
        } else if (unix >= (uint32_t)RTC_Months[0][tm->Month]) {
            unix -= RTC_Months [0][tm->Month];
        } else {
            break;
        }
    }

    tm->Month++;            /* Month starts with 1 */
    tm->DayOfmonth = unix + 1;     /* Date starts with 1 */
}

#endif //RTC_ENABLE
