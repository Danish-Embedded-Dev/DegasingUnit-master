#ifdef USE_ETHERNET


//---------------connect to network with Dynamic_IP--------
bool ConnectWithDHCP() {
  // start the Ethernet connection:
  DEBUG_PRINTLN("Initialize Ethernet with DHCP with unique MAC:");

  if (!restart_count) { //only once at restart
    //update new mac address from unique_ID of stm32f103
    saveMAC(mac);

    String f[3];
    for (int i = 0 ; i < 3 ; i++) { //using these to overcome the starting zero issue e.g 0x0d appear as d only
      mac[i + 3] <= 0x0F ?  f[i] = 0 + String(mac[i + 3], HEX) :  f[i] =  String(mac[i + 3], HEX) ;
    }

    //print last three digit of mac in client name
    Client_Name = HOST_NAME  + f[0] + f[1] + f[2];
  }  

  if (Ethernet.begin(mac) == 0) {
    DEBUG_PRINTLN("Failed to configuration");
    byte static_ip[] = {192, 168, 4, 1}; //local ip in lan
    Ethernet.begin(mac, static_ip);
    DEBUG_PRINT("static ip address: ");
    DEBUG_PRINTLN(Ethernet.localIP());
    return false;
  }
  // print your local IP address:
  DEBUG_PRINT("My IP address: ");
  DEBUG_PRINTLN(Ethernet.localIP()); 
  Serial.println(Ethernet.localIP()); 
  return true;
}


//generating mac address from controller UNIQUI_ID
void saveMAC(byte * mac) {
  for (int i = 2; i >= 0; i--) {
    mac[5 - i] = (*(volatile byte *)(U_ID + i));
    mac[2 - i] = (*(volatile byte *)(U_ID + i + 9));
  }
  // for Custom MAC i.e. "Locally Administered Addresses", second LSB should be HIGh and first LSB should be LOW (Unicast transmission)
  mac[0] = 0xFE;
}

//---------------Setting NTP server ----------------------
#ifdef NTP_ENABLE
//void Setup_Ntp_Server() {
//
//#ifdef  ExEEPROM_ENABLE
//  timeClient.setPoolServerName(running_val.NTPserver.c_str());
//#endif//ExEEPROM_ENABLE 
//  timeClient.begin();
//
//
//  //Update RTC from NTP packet
//  int timeout_ms = 1000;
//  unsigned long polling_tm = millis();
//  while (!NTP_status &&  (millis() - polling_tm < timeout_ms)) { //keep trying untill time is not getting from server or timeout
//    NTP_status = timeClient.update();//get update from server
//  }
//
//#ifdef  RTC_ENABLE
//  if (NTP_status == true) { //update RTC only when network success
//    setEpoch(timeClient.getEpochTime()); //store the current time to RTC 
//  }
//#endif//RTC_ENABLE 
//
//  DEBUG_PRINTLN("------------NTP-Server--------------");
//  DEBUG_PRINT("NTP_Time_Update :"); DEBUG_PRINTLN(NTP_status ? "success" : "failed");
//
//  /* Release any resources being used by this EthernetUDP ntp instance */
//  ntpUDP.stop();   //it will be use for UDP transition event update
//
//
//}
#endif//NTP_ENABLE


#endif //USE_ETHERNET
