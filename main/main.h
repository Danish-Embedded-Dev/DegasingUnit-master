// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions

#ifndef _FX1U_INSPECTION_MONITOR_
#define _FX1U_INSPECTION_MONITOR_
#include "Arduino.h"
#include "conf.h"
 

//-----------------------------------------
#ifdef  USE_ETHERNET
  #include <SPI.h> 
//  #ifdef  USE_SPI2  // in this module it's set by core library
//  SPIClass SPI(2);
//  #else
//  SPIClass SPI(1);
//  #endif//USE_SPI2
  
  #include <Ethernet_STM.h>
  
#endif//USE_ETHERNET

//----------------------
#ifdef CONFIG_PAGE_ENABLE
  EthernetServer Local_server(7104); //server port
#endif//CONFIG_PAGE_ENABLE

//-----------------------------------------
#ifdef  MQTT_ENABLE
  #include <PubSubClient.h> 
  #include <ArduinoJson.h>
  //Class instants
  EthernetClient ethClient;
  PubSubClient mqttclient(ethClient);
#endif//MQTT_ENABLE

//-----------------------------------------
//UDP Alert packet 
#include <EthernetUdp.h>  
  //Class instants  
EthernetUDP eventUDP;  
//-----------------------------------------

//-----------------------------------------
#ifdef  NTP_ENABLE
//  #include <NTPClient.h>
  
  EthernetUDP ntpUDP; 
  
//#ifdef  ExEEPROM_ENABLE
//  NTPClient timeClient(ntpUDP,default_val.NTPserver.c_str(),0,default_val.NTPinterval);//update ntp parameters
//#else
//  NTPClient timeClient(ntpUDP, NTPserver.c_str(),0,NTPinterval);//update ntp parameters 
//#endif//ExEEPROM_ENABLE
  
#endif//NTP_ENABLE

//-----------------------------------------
#ifdef  WATCHDOG_ENABLE
  #include <libmaple/iwdg.h>
#endif//WATCHDOG_ENABLE


//-----------------------------------------
#ifdef I2C_ENABLE 
  #include <Wire.h>
  
  #ifdef USE_I2C2  //if want to use i2c alternate pins for i2c communication
  TwoWire WIRE2 (2,I2C_FAST_MODE);
  #define Wire WIRE2
  #endif//USE_I2C2
  
#endif//I2C_ENABLE

//-----------------------------------------
#ifdef  ExEEPROM_ENABLE
    #include "ExEEPROM_lib/EE_lib.h"  
    ExEEPROM EE(EE_ADDR); //eeprom instance creation
#endif//ExEEPROM_ENABLE


//-----------------------------------------

#ifdef  INTL_RTC_ENABLE
    #include <RTClock.h>
    RTClock intl_rtc (RTCSEL_LSE); // initialise
#endif//IN_RTC_ENABLE

//-----------------------------------------
#ifdef  MODULE_CURRENT_EVENT
    #include "EmonLib.h"                   // Include Emon Library
    EnergyMonitor emon1;                   // Create an instance
#endif//MODULE_CURRENT_EVENT

#endif // _FX1U_INSPECTION_MONITOR_ 
