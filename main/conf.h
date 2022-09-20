#ifndef CONF_H_
#define CONF_H_

//#define DEBUGOUT
#define USE_SERIAL
//  #define USE_SERIAL1
//  #define USE_SERIAL2
//  #define USE_SERIAL3 
#define USE_ETHERNET    //spi2 is using from library spi.c and 5500.c rogarclark
  #define USE_SPI2     //uncomment will switch to spi_1[note use only with Ethernet_STM.h buildin RogarClark improved Spi.c ]
//#define USE_MODBUS 
//  #define ModSerial Serial2  

#define DHCP_ENABLE
//#define STATICIP_ENABLE
#define MQTT_ENABLE
//#define NTP_ENABLE
#define WATCHDOG_ENABLE
#define I2C_ENABLE
#define USE_I2C2  //uncomment will automatically shift to I2C_1
#define ExEEPROM_ENABLE
#define RTC_ENABLE
#define INTL_RTC_ENABLE
#define CONFIG_PAGE_ENABLE 

//------------MODULE----------
#define MODULE_CURRENT_EVENT

//-------GLOBAL_VAR-----------
 
 
//-------ETHERNET_VAR------------
#ifdef USE_ETHERNET
  // Use assigned MAC address of WIZ550io
  byte mac[] = {0xDE, 0xAD, 0xBE, 0x08, 0x2b, 0x14};
  // Use assigned IP address of WIZ550io 
  byte static_ip[] = {192,168,4,1};  
  String Client_Name   = "Vectra" ; //same as HOST_Name 
  //link down variable for physical state check
  #define _LINK_DOWN_   186        // physical status of link  
  #define  U_ID         0x1ffff7e8
  bool Link_up = false;
  #define Rst_pin  PB1
#endif //USE_ETHERNET  
  
//-------MQTT_VAR------------
#ifdef MQTT_ENABLE
  
  String Sub_Topic          = "atlasautos/xx/cm/xx";    
  String Sub_Topic_Mod      = "atlasautos/xx/cm/xx/mod";               
  String Sub_Topic_DBG      = Client_Name + "/extensity";  //USING FOR FIXED TOPIC CREATING FOR DEBUGGING

  String Pub_Topic_OUT      = Sub_Topic + "/out";           //topic for normal operation
  String Pub_Topic_Mod_OUT  = Sub_Topic + "/mod/" + "/out"; //topic for normal operation
  String Pub_Topic_RES      = Sub_Topic + "/res";           //topic for response of general query                   
  String Pub_Topic_DBG      = Sub_Topic + "/dbg";           //topic for debugging
  String Pub_Topic_LW       = Sub_Topic + "/lwt";   //topic for LW
  String Pub_Topic_BIRTH    = Sub_Topic + "/birth";//topic for Device startup ALERT
  String Pub_Topic_RES_DBG  = Sub_Topic_DBG + "/res"; 
   
  String will_Message = "{\"device_id\":\"" + Client_Name + "\",\"status\" : \"disconnect\"}";
  
  unsigned long Reconnect_tm = 5000; //5 seconds 

  uint16_t restart_count = 0; //for checking reconnection because of restart or by internet lost

  #define OFFLINE 0  //if sending packet from eeprom 
  #define ONLINE  1  //if sending packet is live
  
  //-----------BLINK_STATUS----------- 
  #define Network_LED PB1     //Network and MQtt
  #define MQTT_PUSH_LED PB0   //packet send to cloud 
  bool PUSH_ENABLE = false;   //use for turn off after 2 tick
  int Link_Blink_Interval = 500;   //500ms 
//  int Mqtt_Blink_Interval = 100;  //use this for interupt routine
  
  //-----------reset-status--------
  uint32_t reset_cnt __attribute__ ((section (".noinit"))); 

  //for checking mqtt status 
  bool mqtt_connected = false;
  
#endif//MQTT_ENABLE

//----------NTP_VAR------------

#ifdef  NTP_ENABLE
  bool NTP_status = false;
  #ifndef ExEEPROM_ENABLE
  String  NTPserver   = "pool.ntp.org"; 
  int     NTPinterval = 1000 ;  //1000ms 
  #endif//ExEEPROM_ENABLE
#endif//NTP_ENABLE


//--------login-Page--------
#ifdef  CONFIG_PAGE_ENABLE
  bool login_page = true;
  unsigned long web_Prev_tm = 0;
  #define config_timeout 10*60   //10min or 600 seconds timeout for configuration mode
  #define config_switch PB9 
#endif//CONFIG_PAGE_ENABLE  

//------------EEPROM Block----------
#ifdef  ExEEPROM_ENABLE
  #define EE_ADDR 0x57        //EEPROM PHYSICAL ADDRESS ON I2C COMMUNICATION LAYER 
  #define EEPROM_Lenght 4095  //EEPROM max length
  #define Reserved_pvt_bytes 160
  #define PAGE_BASE_ADDR 160 
  #define PAGE_SIZE 32
  #define TOTAL_PAGES (EEPROM_Lenght - PAGE_BASE_ADDR + 1 )/PAGE_SIZE
  
  //structure variable
  struct Byte_type{
      uint32_t unix_time; 
      double Irms;  
  }; Byte_type EEPROM_DATA;
  
  struct  Default_variable{  
      int NTPinterval = 1000 ;  //1000ms 
      int dynamic_data_Ptr = 0;
      int mqtt_port = 31883; //atlas default port address
//      int mqtt_port = 1883; //default port address
      String  NTPserver = "pool.ntp.org";
      String  mqtt_server = "10.7.1.111"; //atlas default
//  String  mqtt_server = "192.168.120.164";
      String CompanyNm = "atlasautos";
      String DepartNm = "xx";
      String DeviceID = "cm";
      String DeviceNo = "01";
      int No_of_Samples = 1092; //1000 sample for Irms
      int Alert_tm = 60;        // in s 
      int Delta_Curr = 50;    //in mA
      int Delta_tm = 2000;      //in ms  
      int CT_Ratio = 1000;      //default is 1 no multiplier
      int Mod_Config = 0;       //default is SERIAL_8N1 from core 
      int Mod_Baud   = 9600;    //default baudrate is 9600
      int Udp_host_port = 4024; //default UDP destination port address
      String Udp_host_ip = "192.168.1.101"; //default UDP destination ip address
      int no_load_current = 150;  //150 mA no load condition
  }default_val,running_val;
  
  enum EepromPrivateAddress {
      _ADDR_NTP_Server     = 10,  //20bytes reserved for NTP_Server
      _ADDR_MQTT_Server    = 30,  //20bytes reserved for MQTT_Server
      _ADDR_Port           = 50,  //10bytes reserved for Port (0-65535)&Checksum  
      _ADDR_CompanyNm      = 60,  //15bytes reserved for CompanyNm
      _ADDR_DepartNm       = 75,  //15bytes reserved for DepartNm
      _ADDR_DeviceNo       = 90,  //15bytes reserved for DeviceNo
      _ADDR_Alert_tm       = 105, //5bytes reserved for Alert_tm
      _ADDR_Delta_Curr     = 110, //5bytes reserved for DeltaCurr
      _ADDR_Delta_tm       = 115, //5bytes reserved for Delta_tm
      _ADDR_CT_Ratio       = 120, //5bytes reserved for Delta_tm
      _ADDR_Mod_Config     = 125, //5bytes reserved for modbus serial mode
      _ADDR_Mod_Baud       = 130, //5bytes reserved for modbus serial baudrate
      _ADDR_UDP_HOST_IP    = 135, //15bytes reserved for udp packet host IP
      _ADDR_UDP_HOST_PORT  = 150, //5bytes reserved for udp packet host port
      _ADDR_NO_LOAD_CURR   = 155, //5bytes reserved for no load variable
  }; 
#endif//ExEEPROM_ENABLE


//------------------RTC-Block---------
#ifdef RTC_ENABLE
  #define DS3231_I2C_ADDRESS 0x68
  typedef struct  { //unix conversion parameters
    uint8_t Second; 
    uint8_t Minute; 
    uint8_t Hour; 
    uint8_t Wday;   // day of week, sunday is day 1
    uint8_t DayOfmonth;
    uint8_t Month; 
    uint8_t Year;   // offset from 1970; 
  }tmElements_t ; 
#endif//RTC_ENABLE


//---------------SERIAL-DEBUG-----------
#define baudrate 9600

#ifdef DEBUGOUT 
 
 #ifdef USE_SERIAL //Use USB_Serial for debug communication
  #define DEBUG_PRINT_F(...)    Serial.print(F(__VA_ARGS__))
  #define DEBUG_PRINTLN_F(...)  Serial.println(F(__VA_ARGS__))
  #define DEBUG_PRINT(...)    Serial.print(__VA_ARGS__)
  #define DEBUG_PRINTLN(...)  Serial.println(__VA_ARGS__)
 #endif 
 
 #ifdef USE_SERIAL1 //Use Seria11 for debug communication
  #define DEBUG_PRINT_F(...)    Serial1.print(F(__VA_ARGS__))
  #define DEBUG_PRINTLN_F(...)  Serial1.println(F(__VA_ARGS__))
  #define DEBUG_PRINT(...)    Serial1.print(__VA_ARGS__)
  #define DEBUG_PRINTLN(...)  Serial1.println(__VA_ARGS__)
 #endif   
 
 #ifdef USE_SERIAL2 //Use Seria12 for debug communication
  #define DEBUG_PRINT_F(...)    Serial2.print(F(__VA_ARGS__))
  #define DEBUG_PRINTLN_F(...)  Serial2.println(F(__VA_ARGS__))
  #define DEBUG_PRINT(...)    Serial2.print(__VA_ARGS__)
  #define DEBUG_PRINTLN(...)  Serial2.println(__VA_ARGS__)
 #endif   

 #ifdef USE_SERIAL3 //Use Seria13 for debug communication
  #define DEBUG_PRINT_F(...)    Serial3.print(F(__VA_ARGS__))
  #define DEBUG_PRINTLN_F(...)  Serial3.println(F(__VA_ARGS__))
  #define DEBUG_PRINT(...)    Serial3.print(__VA_ARGS__)
  #define DEBUG_PRINTLN(...)  Serial3.println(__VA_ARGS__)
 #endif  
#else
  #define DEBUG_PRINT_F(...) //else remove the debug  feature
  #define DEBUG_PRINTLN_F(...)
  #define DEBUG_PRINT(...)
  #define DEBUG_PRINTLN(...)
#endif //DEBUGOUT  


//------------------MODULE_CURRENT_EVENT-----------
#ifdef MODULE_CURRENT_EVENT
  
  byte TIMER_INTERVAL = 200;  //200ms
   
  bool mqtt_packet_update = false; //use for packet send to mqtt or not 

  bool udp_packet_update  = false; //use for packet send to udp or not 
  
  bool alert_status = LOW;   //use for periodic or alert packet

  double LastReading = 100;    //use for current last reading store normal set to 0.1A = 100mA

  bool forced_zero_packet =  false;
  
  unsigned long master_tick = 0; 
#endif//MODULE_CURRENT_EVENT

#endif // CONF_H_  
