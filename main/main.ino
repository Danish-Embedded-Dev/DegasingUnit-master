#include "main.h"


void setup() {  
#ifdef  WATCHDOG_ENABLE
                  iwdg_init(IWDG_PRE_256, 3500); //watch-dog 5000*0.0064 = 32000 ms
#endif//WATCH_ENABLE
#ifdef I2C_ENABLE
                  Setup_I2C();
#endif//I2C_ENABLE  
#ifdef  ExEEPROM_ENABLE
                  Init_EEPROM_Var(); //get EEPROM store parameter
#endif//ExEEPROM_ENABLE  
#ifdef  DEBUGOUT
                  SetupSerialOut();
#endif//DEBUGOUT    

#ifdef  USE_MODBUS  
                  ModSerial.begin(running_val.Mod_Baud,running_val.Mod_Config); 
#endif//USE_MODBUS  
#ifdef  MODULE_CURRENT_EVENT
                  Config_IO();
#endif//MODULE_CURRENT_EVENT  
#ifdef  DHCP_ENABLE
                  ConnectWithDHCP();
#endif//DHCP_ENABLE     
#ifdef  CONFIG_PAGE_ENABLE
                  Local_server.begin();
#endif//CONFIG_PAGE_ENABLE 
#ifdef  NTP_ENABLE
                  Setup_Ntp_Server();
#endif//NTP_ENABLE
                  eventUDP.begin(8888);  //using UDP packet recieveing port 8888
#ifdef  MQTT_ENABLE
                  Setup_Mqtt_Server(); 
#endif//MQTT_ENABLE     
#ifdef  MODULE_CURRENT_EVENT
                  set_Dynamic_ptr();
                  emon1.current(0,17,1.402,-0.03516);    // Current: input pin, calibration.
//                  emon1.current(0,17);    // Current: input pin, calibration.
                  unsigned long pr = millis();
                  while(millis()-pr<1000){
                    LastReading = emon1.calcIrms(running_val.No_of_Samples);
                  }
                  Enable_Timer_Interrupt(TIMER_INTERVAL);
#endif//MODULE_CURRENT_EVENT   
}

void loop() { 
#ifdef  WATCHDOG_ENABLE
  iwdg_feed(); //Feeding watch dog
#endif//WATCH_ENABLE   
 
#ifdef CONFIG_PAGE_ENABLE
  //if button is pressed wait for 10 second to respond the http resquest if any
  unsigned long cur_tm = millis();
  int Respond_timeout = 10; //wait for 10 milli second  
  
  if (!digitalRead(config_switch)) {
     delay(200);
     if(!digitalRead(config_switch)){ //validation check or debounce
      Respond_timeout = config_timeout*1000; //wait for 120*1000 milli second  = 120 second = 2 min
     }
  } 
    
  while ((millis() - cur_tm) <  Respond_timeout ) {
        //if tcp-client is there launch a webpage
        EthernetClient client = Local_server.available();
        if (client) {
          Local_config_mode(); 
          //Timer2.resume();
        }
        if (!login_page) { //turn off config page access after timeout
          if ((millis() - web_Prev_tm) > (config_timeout * 1000)) {
            login_page = true;
            DEBUG_PRINTLN("config_timeout");
            break;
          }
        }
        // always feed the watchdog in blocking area
        #ifdef  WATCHDOG_ENABLE
          iwdg_feed(); //Feeding watch dog
        #endif//WATCH_ENABLE   
  }
  Timer2.resume();  
#endif//CONFIG_PAGE_ENABLE 

 
#ifdef  MQTT_ENABLE
  //---------Network-Status-Polling-----
  Link_up = bitRead(W5100.getPHYCFGR(), 0); //link up for ethernet
  mqtt_connected = mqttclient.connected();
  //--link and mqtt reconnection status -------------
  if (!mqtt_connected) {
    Reconnect(1);
  } 
  mqttclient.loop(); //mqttclient connected 
#endif//MQTT_ENABLE
 
#ifdef MODULE_CURRENT_EVENT

  if (Link_up && mqtt_connected) {
    //-------if-detection---------
    if (mqtt_packet_update) {
      mqtt_publish(EEPROM_DATA, ONLINE, alert_status);
      mqtt_packet_update = false; //packet sended
    }
     
    //At reconnection if data is in eeprom
    if (running_val.dynamic_data_Ptr > 0) { //check data changed or not
      EEPROM_READ(); //read EEPROM and send to mqtt
      delay(10);
    }
  }
  
  if (Link_up){
    if(udp_packet_update){
        //At no load condition current will be zero
        if (EEPROM_DATA.Irms <= (((float)running_val.no_load_current/1000)* ((float)running_val.CT_Ratio/1000))) { //at normal condition 0.05A which convert to 0.1
          EEPROM_DATA.Irms = 0;
        }
        eventUDP.beginPacket(running_val.Udp_host_ip.c_str(), running_val.Udp_host_port);
        char UDPpacket[60];
        sprintf(UDPpacket, "%d : %d", EEPROM_DATA.unix_time,int(EEPROM_DATA.Irms*1000) );//to convert Ampere to milliampere parameter * by 1000 
        eventUDP.write(UDPpacket);
        eventUDP.write("\r\n");
        eventUDP.endPacket();
        udp_packet_update = false; //packet sended
        ethClient.interupt_event = false;
    }
  }    
 
  
#endif//MODULE_CURRENT_EVENT 

#ifdef USE_MODBUS
  //recieving data from modbus device
  if (ModSerial.available() > 0) {
    byte max_length = 200;
    byte Rx_data[max_length];
    serial_capture(Rx_data, &max_length);
    //    DEBUG_PRINTLN(Rx_data[0]);
    mqttclient.publish(Pub_Topic_Mod_OUT.c_str(), Rx_data, max_length);
  }
#endif//MODULE_CURRENT_EVENT   
}
