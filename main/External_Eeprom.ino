  //---------------Setting EEPROM ----------------------
#ifdef ExEEPROM_ENABLE
/*
   Init_EEPROM_Var will re-initialize the variable from EEPROM save data
*/
void Init_EEPROM_Var() {

  EE.eeprom_connected();  //checking EEPROM is connected or not 
  
  String Temp_mqtt_server = EE.eeprom_read_String(_ADDR_MQTT_Server);
  if (Temp_mqtt_server != "-1" ) {
    running_val.mqtt_server = Temp_mqtt_server;
  } else {
    EE.eeprom_write_String(_ADDR_MQTT_Server, default_val.mqtt_server );
    running_val.mqtt_server = default_val.mqtt_server;
  }

  int Temp_port = EE.eeprom_read_Int(_ADDR_Port);
  if (Temp_port != -1 ) {
    running_val.mqtt_port = Temp_port;
  } else {
    EE.eeprom_write_Int(_ADDR_Port, default_val.mqtt_port);
    running_val.mqtt_port = default_val.mqtt_port;
  }

  String Temp_NTP_server = EE.eeprom_read_String(_ADDR_NTP_Server);
  if (Temp_NTP_server != "-1" ) {
    running_val.NTPserver = Temp_NTP_server;
  } else {
    EE.eeprom_write_String(_ADDR_NTP_Server, default_val.NTPserver);
    running_val.NTPserver = default_val.NTPserver ;
  }

  String Temp_CompanyNm = EE.eeprom_read_String(_ADDR_CompanyNm);
  if (Temp_CompanyNm != "-1"  ) {
    running_val.CompanyNm = Temp_CompanyNm;
  } else {
    EE.eeprom_write_String(_ADDR_CompanyNm, default_val.CompanyNm);
    running_val.CompanyNm = default_val.CompanyNm ;
  }

  String Temp_DepartNm = EE.eeprom_read_String(_ADDR_DepartNm);
  if (Temp_DepartNm != "-1" ) {
    running_val.DepartNm = Temp_DepartNm;
  } else {
    EE.eeprom_write_String(_ADDR_DepartNm, default_val.DepartNm);
    running_val.DepartNm = default_val.DepartNm ;
  }

  String Temp_DeviceNo = EE.eeprom_read_String(_ADDR_DeviceNo);
  if (Temp_DeviceNo != "-1" ) {
    running_val.DeviceNo = Temp_DeviceNo;
  } else {
    EE.eeprom_write_String(_ADDR_DeviceNo, default_val.DeviceNo);
    running_val.DeviceNo = default_val.DeviceNo ;
  }

  int Temp_Alert = EE.eeprom_read_Int(_ADDR_Alert_tm);
  if (Temp_Alert != -1 ) {
    running_val.Alert_tm = Temp_Alert;
  } else {
    EE.eeprom_write_Int(_ADDR_Alert_tm, default_val.Alert_tm);
    running_val.Alert_tm = default_val.Alert_tm;
  }

  int Temp_Delta_Curr = EE.eeprom_read_Int(_ADDR_Delta_Curr);
  if (Temp_Delta_Curr != -1 ) {
    running_val.Delta_Curr = Temp_Delta_Curr;
  } else {
    EE.eeprom_write_Int(_ADDR_Delta_Curr, default_val.Delta_Curr);
    running_val.Delta_Curr = default_val.Delta_Curr;
  }

  int Temp_Delta_tm = EE.eeprom_read_Int(_ADDR_Delta_tm);
  if (Temp_Delta_tm != -1 ) {
    running_val.Delta_tm = Temp_Delta_tm;
  } else {
    EE.eeprom_write_Int(_ADDR_Delta_tm, default_val.Delta_tm);
    running_val.Delta_tm = default_val.Delta_tm;
  }

  int Temp_CT_Ratio = EE.eeprom_read_Int(_ADDR_CT_Ratio);
  if (Temp_CT_Ratio != -1 ) {
    running_val.CT_Ratio = Temp_CT_Ratio;
  } else {
    EE.eeprom_write_Int(_ADDR_CT_Ratio, default_val.CT_Ratio);
    running_val.CT_Ratio = default_val.CT_Ratio;
  }

  int Temp_Mod_Config = EE.eeprom_read_Int(_ADDR_Mod_Config);
  if (Temp_Mod_Config != -1 ) {
    running_val.Mod_Config = Temp_Mod_Config;
  } else {
    EE.eeprom_write_Int(_ADDR_Mod_Config, default_val.Mod_Config);
    running_val.Mod_Config = default_val.Mod_Config;
  }

  int Temp_Mod_Baud = EE.eeprom_read_Int(_ADDR_Mod_Baud);
  if (Temp_Mod_Baud != -1 ) {
    running_val.Mod_Baud = Temp_Mod_Baud;
  } else {
    EE.eeprom_write_Int(_ADDR_Mod_Baud, default_val.Mod_Baud);
    running_val.Mod_Baud = default_val.Mod_Baud;
  }

  int Temp_udp_port = EE.eeprom_read_Int(_ADDR_UDP_HOST_PORT);
  if (Temp_udp_port != -1 ) {
    running_val.Udp_host_port = Temp_udp_port;
  } else {
    EE.eeprom_write_Int(_ADDR_UDP_HOST_PORT, default_val.Udp_host_port);
    running_val.Udp_host_port = default_val.Udp_host_port;
  }

  String Temp_udp_ip = EE.eeprom_read_String(_ADDR_UDP_HOST_IP);
  if (Temp_udp_ip != "-1" ) {
    running_val.Udp_host_ip = Temp_udp_ip;
  } else {
    EE.eeprom_write_String(_ADDR_UDP_HOST_IP, default_val.Udp_host_ip );
    running_val.Udp_host_ip = default_val.Udp_host_ip;
  }

  int Temp_no_load_curr = EE.eeprom_read_Int(_ADDR_NO_LOAD_CURR);
  if (Temp_no_load_curr != -1 ) {
    running_val.no_load_current = Temp_no_load_curr;
  } else {
    EE.eeprom_write_Int(_ADDR_NO_LOAD_CURR, default_val.no_load_current);
    running_val.no_load_current = default_val.no_load_current;
  }
 
//  Serial.println("------------EEPROM_Var--------------");
//  Serial.print("NTP_server   :");  Serial.println(Temp_NTP_server);
//  Serial.print("Mqtt_server  :");  Serial.println(Temp_mqtt_server);
//  Serial.print("Mqtt_port    :");  Serial.println(Temp_port); 
//  Serial.print("Udp_host_ip  :");  Serial.println(Temp_udp_ip);
//  Serial.print("Udp_host_port:");  Serial.println(Temp_udp_port);
//  Serial.print("CompanyNm    :");  Serial.println(Temp_CompanyNm);
//  Serial.print("DepartNm     :");  Serial.println(Temp_DepartNm);
//  Serial.print("DeviceNo     :");  Serial.println(Temp_DeviceNo);
//  Serial.print("Alert_tm     :");  Serial.println(Temp_Alert);
//  Serial.print("Delta_Curr   :");  Serial.println(Temp_Delta_Curr);
//  Serial.print("Delta_tm     :");  Serial.println(Temp_Delta_tm);
//  Serial.print("CT_Ratio     :");  Serial.println(Temp_CT_Ratio);
//  Serial.print("Mod_Config   :");  Serial.println(Temp_Mod_Config);
//  Serial.print("Mod_Baud     :");  Serial.println(Temp_Mod_Baud);
//  Serial.print("No_load_Curr :");  Serial.println(Temp_no_load_curr);
//  delay(20); //using because data lost issue without delay
//  Serial.println("------------Running_Var--------------");
//  Serial.print("NTP_server   :");  Serial.println(running_val.NTPserver);
//  Serial.print("Mqtt_server  :");  Serial.println(running_val.mqtt_server);
//  Serial.print("Mqtt_port    :");  Serial.println(running_val.mqtt_port);
//  Serial.print("Udp_host_ip  :");  Serial.println(running_val.Udp_host_ip);
//  Serial.print("Udp_host_port:");  Serial.println(running_val.Udp_host_port);
//  Serial.print("dynamic_ptr  :");  Serial.println(running_val.dynamic_data_Ptr); 
//  Serial.print("CompanyNm    :");  Serial.println(running_val.CompanyNm);
//  Serial.print("DepartNm     :");  Serial.println(running_val.DepartNm);
//  Serial.print("DeviceID     :");  Serial.println(running_val.DeviceID);
//  Serial.print("DeviceNo     :");  Serial.println(running_val.DeviceNo);
//  Serial.print("Alert_tm     :");  Serial.println(running_val.Alert_tm);
//  Serial.print("Delta_Curr   :");  Serial.println(running_val.Delta_Curr);
//  Serial.print("Delta_tm     :");  Serial.println(running_val.Delta_tm);
//  Serial.print("CT_Ratio     :");  Serial.println(running_val.CT_Ratio);
//  Serial.print("Mod_config   :");  Serial.println(running_val.Mod_Config);
//  Serial.print("Mod_Baud     :");  Serial.println(running_val.Mod_Baud);
//  Serial.print("No_load_Curr :");  Serial.println(running_val.no_load_current);
//  delay(20);
//  Serial.println("------------Default_Var--------------");
//  Serial.print("NTP_server   :");  Serial.println(default_val.NTPserver);
//  Serial.print("Mqtt_server  :");  Serial.println(default_val.mqtt_server);
//  Serial.print("Mqtt_port    :");  Serial.println(default_val.mqtt_port);
//  Serial.print("Udp_host_ip  :");  Serial.println(default_val.Udp_host_ip);
//  Serial.print("Udp_host_port:");  Serial.println(default_val.Udp_host_port);
//  Serial.print("dynamic_ptr  :");  Serial.println(default_val.dynamic_data_Ptr); 
//  Serial.print("CompanyNm    :");  Serial.println(default_val.CompanyNm);
//  Serial.print("DepartNm     :");  Serial.println(default_val.DepartNm);
//  Serial.print("DeviceID     :");  Serial.println(default_val.DeviceID);
//  Serial.print("DeviceNo     :");  Serial.println(default_val.DeviceNo);
//  Serial.print("Alert_tm     :");  Serial.println(default_val.Alert_tm);
//  Serial.print("Delta_Curr   :");  Serial.println(default_val.Delta_Curr);
//  Serial.print("Delta_tm     :");  Serial.println(default_val.Delta_tm);
//  Serial.print("CT_Ratio     :");  Serial.println(default_val.CT_Ratio);
//  Serial.print("Mod_config   :");  Serial.println(default_val.Mod_Config);
//  Serial.print("Mod_Baud     :");  Serial.println(default_val.Mod_Baud);
//  Serial.print("No_load_Curr :");  Serial.println(default_val.no_load_current);
  
//  delay(20);
}

void set_Dynamic_ptr() {
  
  int dynamic_addr_ptr;
  byte data_header ;
  
  //check dynamic ptr
  for (int i = 0; i < TOTAL_PAGES; i++) {
    dynamic_addr_ptr = PAGE_BASE_ADDR + (i*PAGE_SIZE);
     
    data_header = EE.eeprom_read_Byte(dynamic_addr_ptr);

    if (data_header == sizeof(EEPROM_DATA)) { 
      running_val.dynamic_data_Ptr = i+1;
    } else {
      break;
    }
  }
}

void EEPROM_WRITE() {
  int dynamic_addr_ptr = PAGE_BASE_ADDR + (running_val.dynamic_data_Ptr * PAGE_SIZE) ;

  if (dynamic_addr_ptr < EEPROM_Lenght) { //when eeprom range full donot add data
    EE.eeprom_write_Obj_P( dynamic_addr_ptr , &EEPROM_DATA, sizeof(EEPROM_DATA));
    running_val.dynamic_data_Ptr++; 
    } 
}

void EEPROM_READ() {
  int dynamic_addr_ptr = PAGE_BASE_ADDR +((running_val.dynamic_data_Ptr - 1) * PAGE_SIZE)   ;

  bool checksum = EE.Eeprom_read_Obj_P(dynamic_addr_ptr, &EEPROM_DATA); //READ DATA FROM EEPROM

  if (checksum == true) { //send data if checksum is true
    
    #ifdef  MQTT_ENABLE
    mqtt_publish(EEPROM_DATA ,OFFLINE,HIGH);
    #endif//MQTT_ENABLE
    
//    Serial.print(running_val.dynamic_data_Ptr); Serial.println(": packet sended success");
  }

  EE.eeprom_write_Byte( dynamic_addr_ptr , 254); //packet disturb with 254  for last ptr
  running_val.dynamic_data_Ptr--;
}



#endif//ExEEPROM_ENABLE
