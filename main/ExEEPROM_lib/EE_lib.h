#ifndef EELIB_H
#define EELIB_H

 

class ExEEPROM {
  private:
    byte DEVADDR ;
    int EEPROM_SIZE;
    

  public:
    int HW_ERROR ; //hardware is connected of not check 
    ExEEPROM(byte Device_address = 0x50, int  eeprom_size = 4095); //default address set to 57 and size 4095bytes

    void eeprom_write_Obj_P(uint16_t address, void *obj, uint8_t Objsize); 
    void eeprom_write_Obj(uint16_t address, void *obj, uint8_t Objsize);
    void eeprom_write_String(int addrOffset, const String &strToWrite);
    void eeprom_write_Page( unsigned eeaddr, const byte * data, byte length);
    void eeprom_write_Bytes(int address, long number , byte sizeofvar);
    void eeprom_write_Long(int address, long  number);
    void eeprom_write_Int(int address, int number);
    void eeprom_write_Byte(unsigned eeaddr,  byte data );

    bool eeprom_connected();
    bool eeprom_clear(byte data = 0xFF);
    void eeprom_show();

    bool Eeprom_read_Obj_P(uint16_t address, void *obj);
    bool Eeprom_read_Obj(uint16_t address, void *obj);
    String eeprom_read_String(int addrOffset);
    long eeprom_read_Long(int address);
    int eeprom_read_Int(int address);
    int eeprom_read_Byte(  unsigned eeaddr);
};

//member defination

//contructor for setting device address
ExEEPROM::ExEEPROM( byte device_address , int  eeprom_size) {
  DEVADDR     = device_address;
  EEPROM_SIZE = eeprom_size;
}

bool  ExEEPROM::eeprom_connected(){
      //HARDWARE ERROR CHECKING
      Wire.beginTransmission(DEVADDR);
      byte error = Wire.endTransmission();
      
      if (error == 0) { //if eeprom is present set true 
        HW_ERROR = false;
        return  true;   
      }else{            //if any error comes set false 
        HW_ERROR = true;
        return false; 
      }
}


//structure variable written with the help of pointer
//------------modify 17/9/2021----------------
void ExEEPROM::eeprom_write_Obj_P(uint16_t address, void *obj, uint8_t Objsize){
   //EEPROM HARDWARE CHECK 
   if(HW_ERROR){  //IF HARDWARE ERROR IS THERE RETURN DO NOTHING
      return ;
    }
  
  uint8_t checksum = 0, *ptr = (uint8_t*)obj;

 
  
  Wire.beginTransmission(DEVADDR);
  Wire.write((int)(address >> 8)); // MSB
  Wire.write((int)(address & 0xFF)); // LSB
  Wire.write(Objsize);//set lenght of data on first address
  
  for (int i = 0; i < Objsize; i++) {
    checksum = (checksum) ^ (*(ptr));
    Wire.write( *(ptr++));
    //delay(0.5); //not used in interupt routine
  }
  Wire.write(checksum); //put checksum at last address
  //delay(6);  // Little delay to assure EEPROM is able to process data; if missing and inside for look meses some values
  Wire.endTransmission();
}

void ExEEPROM::eeprom_write_Obj(uint16_t address, void *obj, uint8_t Objsize){
  //EEPROM HARDWARE CHECK 
   if(HW_ERROR){  //IF HARDWARE ERROR IS THERE RETURN DO NOTHING
      return ;
    }
    
  uint8_t checksum = 0, *ptr = (uint8_t*)obj;
  eeprom_write_Byte(address++, Objsize); //set lenght of data on first address
  for (int i = 0; i < Objsize; i++) {
    checksum = (checksum) ^ (*(ptr));
    eeprom_write_Byte(address++, *(ptr++));
  }
  eeprom_write_Byte(address, checksum); //put checksum at last address

}


void ExEEPROM::eeprom_write_String(int addrOffset, const String &strToWrite) {
  //EEPROM HARDWARE CHECK 
   if(HW_ERROR){  //IF HARDWARE ERROR IS THERE RETURN DO NOTHING
      return ;
    }
    
  byte len      = strToWrite.length();
  byte checksum = 0;

  eeprom_write_Byte(addrOffset, len); //set lenght of data on first address

  for (int i = 0; i < len; i++) { //Dumping Data
    eeprom_write_Byte(addrOffset + 1 + i, strToWrite[i]);
    checksum ^= strToWrite[i];
  }
  eeprom_write_Byte(addrOffset + len + 1, checksum);
}

void ExEEPROM::eeprom_write_Page( unsigned eeaddr, const byte * data, byte length)
{
  //EEPROM HARDWARE CHECK 
   if(HW_ERROR){  //IF HARDWARE ERROR IS THERE RETURN DO NOTHING
      return ;
    }
    
  byte deviceaddress = DEVADDR;
  // Three lsb of Device address byte are bits 8-10 of eeaddress
  byte devaddr = deviceaddress | ((eeaddr >> 8) & 0x07);
  byte addr    = eeaddr;
  Wire.beginTransmission(devaddr);
  Wire.write(int(addr));
  for (int i = 0; i < length; i++) {
    Wire.write(data[i]);
  }
  Wire.endTransmission();
  delay(10);
}

void ExEEPROM::eeprom_write_Bytes(int address, long number , byte sizeofvar)
{
  //EEPROM HARDWARE CHECK 
   if(HW_ERROR){  //IF HARDWARE ERROR IS THERE RETURN DO NOTHING
      return ;
    }
    
  byte checksum = 0;

  // write to eeprom in byte format
  for ( int i = 0; i < sizeofvar ; i++) {
    eeprom_write_Byte(address + i, (number >> 8 * ((sizeofvar - 1) - i) & 0xFF));
  }

  //calculation of checksum
  for ( int i = 0; i < sizeofvar ; i++) {
    checksum ^= ((number >> (8 * i)) & 0xFF);
  }

  eeprom_write_Byte(address + sizeofvar, checksum);
}

void ExEEPROM::eeprom_write_Long(int address, long  number)
{
  //EEPROM HARDWARE CHECK 
   if(HW_ERROR){  //IF HARDWARE ERROR IS THERE RETURN DO NOTHING
      return ;
    }
    
  eeprom_write_Byte(address, (number >> 24) & 0xFF);
  eeprom_write_Byte(address + 1, (number >> 16) & 0xFF);
  eeprom_write_Byte(address + 2, (number >> 8) & 0xFF);
  eeprom_write_Byte(address + 3, number & 0xFF);
  eeprom_write_Byte(address + 4, ((number >> 24) & 0xFF) ^ ((number >> 16) & 0xFF) ^ ((number >> 8) & 0xFF) ^ (number & 0xFF));
}

void ExEEPROM::eeprom_write_Int(int address, int number)
{
  //EEPROM HARDWARE CHECK 
   if(HW_ERROR){  //IF HARDWARE ERROR IS THERE RETURN DO NOTHING
      return ;
    }
    
  eeprom_write_Byte(address , (number >> 8) & 0xFF);
  eeprom_write_Byte(address + 1, number & 0xFF);
  eeprom_write_Byte(address + 2, ((number >> 8) & 0xFF) ^ (number & 0xFF)); //checksum
}


void ExEEPROM::eeprom_write_Byte( unsigned eeaddr,  byte data ) {
  //EEPROM HARDWARE CHECK 
   if(HW_ERROR){  //IF HARDWARE ERROR IS THERE RETURN DO NOTHING
      return ;
    }
    
  Wire.beginTransmission(DEVADDR);
  Wire.write((int)(eeaddr >> 8)); // MSB
  Wire.write((int)(eeaddr & 0xFF)); // LSB
  Wire.write(data);
  delay(6);  // Little delay to assure EEPROM is able to process data; if missing and inside for look meses some values
  Wire.endTransmission();
  delay(10);
}

void ExEEPROM::eeprom_show(){ 
  //EEPROM HARDWARE CHECK 
   if(HW_ERROR){  //IF HARDWARE ERROR IS THERE RETURN DO NOTHING
      return ;
    }
    
   int pages = EEPROM_SIZE/16; //divide into pages  
     
   for( int addr = 0; addr <= pages ; addr++){
     Wire.beginTransmission(DEVADDR);
     Wire.write((int)((addr*16) >> 8));   // MSB
     Wire.write((int)((addr*16) & 0xFF)); // LSB 
     delay(6);
     Wire.endTransmission();

     Wire.requestFrom(int(DEVADDR), 16);
     for(int i=0; i<16; i++){
      if (Wire.available()) {
           Serial.print(Wire.read()); Serial.print(" ");
      }
      delay(1); 
     }
     Serial.println();
     delay(1);       
   } 
}

bool ExEEPROM::eeprom_clear(byte data  ) {
  //EEPROM HARDWARE CHECK 
   if(HW_ERROR){  //IF HARDWARE ERROR IS THERE RETURN DO NOTHING
      return false;
    }
    
  int pages = EEPROM_SIZE / 16; //divide into pages
 
  for ( int addr = 0; addr <= pages ; addr++) {
    Wire.beginTransmission(DEVADDR);
    Wire.write((int)((addr * 16) >> 8)); // MSB
    Wire.write((int)((addr * 16) & 0xFF)); // LSB 
    for (int i = 0; i < 16; i++) { //send 16 data
      Wire.write(data);
    }
    delay(10);
    Wire.endTransmission();
  }
  return true;
}

//structure variable read with the help of pointer
//------------modify 2/18/2022----------------
bool ExEEPROM::Eeprom_read_Obj_P(uint16_t address, void *obj)
{

  //EEPROM HARDWARE CHECK 
   if(HW_ERROR){  //IF HARDWARE ERROR IS THERE RETURN DO NOTHING
      return false;
    }
    
  byte i, *ptr = (byte *)obj;
  byte Objsize = eeprom_read_Byte(  address++);
  byte checksum = 0;
  byte Eeprombytes[Objsize]; // to store the eeprom pointed value

  if(Objsize > 32){ //if first address contain size of data more then 32 byte
    return false;
  }
  
  Wire.beginTransmission(DEVADDR);
  Wire.write((int)(address >> 8)); // MSB
  Wire.write((int)(address & 0xFF)); // LSB
  delay(6);
  Wire.endTransmission();
  
  Wire.requestFrom(int(DEVADDR), Objsize);
  
  //Calculation of the checksum
  for (i = 0; i < Objsize;i++) {
    if (Wire.available()) {
      *(ptr) = Wire.read();
      checksum = (checksum) ^ ( *(ptr++)); 
      delay(0.5);
    }
  }  
    
  //     checksum verification
  if (checksum == eeprom_read_Byte(address+Objsize)) {
    //checksum is correct
    return true;
  } else {
    return false;
  }

}
//------------modify 17/9/2021----------------
bool ExEEPROM::Eeprom_read_Obj(uint16_t address, void *obj)
{
  //EEPROM HARDWARE CHECK 
   if(HW_ERROR){  //IF HARDWARE ERROR IS THERE RETURN DO NOTHING
      return false;
    }
    
  byte i, *ptr = (byte *)obj;
  byte Objsize = eeprom_read_Byte(  address++);
  byte checksum = 0;
  byte Eeprombytes[Objsize]; // to store the eeprom pointed value

  if(Objsize > 32){ //if first address contain size of data more then 32 byte
    return false;
  }
   
  //      Calculation of the checksum
  for (i = 0; i < Objsize; i++) {
    *(ptr) = eeprom_read_Byte(  address++);
    checksum = (checksum) ^ ( *(ptr++));
  }

  //     checksum verification
  if (checksum == eeprom_read_Byte(address)) {
    //checksum is correct
    return true;
  } else {
    return false;
  }

}


String ExEEPROM::eeprom_read_String(int addrOffset) {

  //EEPROM HARDWARE CHECK 
   if(HW_ERROR){  //IF HARDWARE ERROR IS THERE RETURN DO NOTHING
      return "-1";
    }
    
  int newStrLen = eeprom_read_Byte(  addrOffset);
  char data[newStrLen + 1];
  byte checksum = 0;

  if (newStrLen > 250) { //for first time read value is 255
    newStrLen = 10;
  }

  for (int i = 0; i < newStrLen; i++) {
    data[i]  = eeprom_read_Byte(  addrOffset + 1 + i);
    checksum ^= data[i];
  }


  data[newStrLen] = '\0';
  byte read_checksum = eeprom_read_Byte(  addrOffset + 1 + newStrLen);

  if (checksum == read_checksum ) {
    return String(data);
  } else {
    return String("-1");
  }

}




long ExEEPROM::eeprom_read_Long(int address)
{ 
  
  //EEPROM HARDWARE CHECK 
   if(HW_ERROR){  //IF HARDWARE ERROR IS THERE RETURN DO NOTHING
      return -1;
    }
    
  //MMSB,MSB,LSB,LLSB SEQUENCE
  byte MMSB      = eeprom_read_Byte(  address);
  byte MSB      = eeprom_read_Byte(  address + 1);
  byte LSB      = eeprom_read_Byte(  address + 2);
  byte LLSB      = eeprom_read_Byte(  address + 3);
  byte checksum = eeprom_read_Byte(  address + 4);

  if ((MMSB ^ MSB ^ LSB ^ LLSB) == checksum ) {
    return ((long)eeprom_read_Byte( address ) << 24) |
           ((long)eeprom_read_Byte( address + 1 ) << 16) |
           ((long)eeprom_read_Byte( address + 2 ) << 8) |
           (long)eeprom_read_Byte( address + 3);
  } else {
    return -1;
  }
}


int ExEEPROM::eeprom_read_Int(int address)
{
  //EEPROM HARDWARE CHECK 
   if(HW_ERROR){  //IF HARDWARE ERROR IS THERE RETURN DO NOTHING
      return -1;
    }
    
  byte MSB      = eeprom_read_Byte(address);
  byte LSB      = eeprom_read_Byte(address + 1);
  byte checksum = eeprom_read_Byte(address + 2);

  if ((MSB ^ LSB) == checksum ) {
    return ((int)eeprom_read_Byte(address ) << 8) +
           (int)eeprom_read_Byte(address + 1);
  } else {
    return -1;
  }
}

int ExEEPROM::eeprom_read_Byte( unsigned eeaddr) {
  //EEPROM HARDWARE CHECK 
   if(HW_ERROR){  //IF HARDWARE ERROR IS THERE RETURN DO NOTHING
      return -1;
    }
    
  byte rdata = -1;

  Wire.beginTransmission(DEVADDR);
  Wire.write((int)(eeaddr >> 8)); // MSB
  Wire.write((int)(eeaddr & 0xFF)); // LSB
  delay(6);
  Wire.endTransmission();
  Wire.requestFrom(int(DEVADDR), 1);
  if (Wire.available()) {
    rdata = Wire.read();
  }
  return rdata;
}


#endif
