#ifdef  MQTT_ENABLE

struct Network_var {
  unsigned long current_tm   = 0;
  unsigned long previous_tm  = -5000;
} mqtt;

void Setup_Mqtt_Server() {
  mqttTopicGenerator();
  mqttclient.setServer(running_val.mqtt_server.c_str(), running_val.mqtt_port);
  mqttclient.setCallback(callback);
  //  Reconnect(1);
}

void mqttTopicGenerator() {
  Sub_Topic  = running_val.CompanyNm + "/" ;
  Sub_Topic += running_val.DepartNm  + "/" ;
  Sub_Topic += running_val.DeviceID  + "/" ;
  Sub_Topic += running_val.DeviceNo  ;
  Sub_Topic_Mod    = Sub_Topic  + "/mod";  //subcribe topic for modbus byte operation
  Sub_Topic_DBG    = Client_Name + "/extensity";

  Pub_Topic_OUT      = Sub_Topic + "/out";  //topic for normal operation
  Pub_Topic_Mod_OUT  = Sub_Topic + "/mod/out"; //topic for modbus byte operation
  Pub_Topic_RES      = Sub_Topic + "/res";  //topic for response of general query
  Pub_Topic_DBG      = Sub_Topic + "/dbg";  //topic for debugging
  Pub_Topic_LW       = Sub_Topic + "/lwt";   //topic for LW
  Pub_Topic_BIRTH    = Sub_Topic + "/birth";//topic for Device startup ALERT
  Pub_Topic_RES_DBG  = Client_Name + "/extensity/res";  //update with new clientName
  will_Message = "{\"device_id\":\"" + Client_Name + "\",\"status\" : \"disconnect\", \"epoch\" :  " + String(getEpoch()) + " }";

  DEBUG_PRINTLN("------GENERATED TOPICS---");
  DEBUG_PRINT("Sub_Topic:"); DEBUG_PRINTLN(Sub_Topic);
  DEBUG_PRINT("Sub_Topic_MOD:"); DEBUG_PRINTLN(Sub_Topic_Mod) ;
  DEBUG_PRINT("Sub_Topic_DBG:"); DEBUG_PRINTLN(Sub_Topic_DBG);
  DEBUG_PRINT("Pub_Topic_OUT:"); DEBUG_PRINTLN(Pub_Topic_OUT);
  DEBUG_PRINT("Pub_Topic_Mod_OUT:"); DEBUG_PRINTLN(Pub_Topic_Mod_OUT);
  DEBUG_PRINT("Pub_Topic_RES:"); DEBUG_PRINTLN(Pub_Topic_RES);
  DEBUG_PRINT("Pub_Topic_DBG:"); DEBUG_PRINTLN(Pub_Topic_DBG);
  DEBUG_PRINT("Pub_Topic_LW:"); DEBUG_PRINTLN(Pub_Topic_LW);
  DEBUG_PRINT("Pub_Topic_BIRTH:"); DEBUG_PRINTLN(Pub_Topic_BIRTH);
  DEBUG_PRINT("Pub_Topic_RES_DBG:"); DEBUG_PRINTLN(Pub_Topic_RES_DBG);

}

/*
   Reconnect to mqtt while disconnect and update the blink status
   fast blink for ethernet(link) down
   slow blink for Mqtt broker lost
*/
void  Reconnect (byte reconnect_state) {

  if (reconnect_state) { //Mqtt connection lost
    mqtt.current_tm = millis();

    if (mqtt.current_tm - mqtt.previous_tm >= Reconnect_tm) { //attempt in 5 seconds
      //----------attemp to physical connection---------------------------
      if (Link_up) { //donot attempt to connect when physically not connected
        /* Release any resources being used by this EthernetUDP ntp instance */
        eventUDP.stop();   //it will be use for UDP transition event update 
        //for mqtt reconnection always reconnect the ethernet for the cases of noise
        ConnectWithDHCP();
        Local_server.begin();
        DEBUG_PRINT("Attempting MQTT connection...");
        eventUDP.begin(running_val.Udp_host_port);  //using UDP packet for event transition push to udp
      }

      //-----------attemp to mqtt-broker connection---------------------------
      if (mqttclient.connect(Client_Name.c_str(), Pub_Topic_LW.c_str(), 0, 0, will_Message.c_str())) {
        digitalWrite(Network_LED, true); //Turn off blinking

        DEBUG_PRINTLN("connected");

        // Once connected, publish an announcement...
        DynamicJsonDocument  JSONencoder(300); //for sending PACKET to MQTT SERVER

        JSONencoder["device_id"]         = Client_Name;
        JSONencoder["con_status"]        = "connected";
//        JSONencoder["ntp_status"]        = NTP_status ;

        if (EE.HW_ERROR) {  //if ds3231 is not there then use internal rtc
          //JSONencoder["con_time"]        = intl_rtc.getTime(); 
          JSONencoder["con_time"]        = "RTC_ERR";
        } else {
          JSONencoder["con_time"]        = getEpoch(); 
        }

        if (!EE.eeprom_connected()) {
          JSONencoder["eeprom"]          = "error" ;
        }

        JSONencoder["con_count"]         = restart_count;
        restart_count++;

        JSONencoder["reset_cnt"]         = reset_cnt;

        JSONencoder["device_ver"]        = "v2.0";

        JSONencoder["upload_date"]       = __DATE__;

        JSONencoder["upload_time"]       = __TIME__;

        if (JSONencoder.size() > 0) { // send online when payload contain any data
          char JSONmessageBuffer[250]; //for sending over mqtt server
          serializeJson(JSONencoder, JSONmessageBuffer);
          mqttclient.publish(Pub_Topic_BIRTH.c_str(), JSONmessageBuffer );
        }

        mqttclient.subscribe(Sub_Topic.c_str()); // ... and resubscribe
        mqttclient.subscribe(Sub_Topic_Mod.c_str()); // ... and resubscribe
        mqttclient.subscribe(Sub_Topic_DBG.c_str()); // ... subcribe for debugging
      } else {
        DEBUG_PRINT("failed, rc=");
        DEBUG_PRINTLN(mqttclient.state());
        DEBUG_PRINTLN("try again in 5 seconds");
        mqtt.previous_tm = millis();
      }
    }
  }
  
        
}


const char* get_cmd = "?";
const char* set_cmd = "set";

void callback(char* topic, byte* payload, unsigned int length) {
  DEBUG_PRINT("Message arrived [");
  DEBUG_PRINT(topic);
  DEBUG_PRINT("] ");

#ifdef USE_MODBUS 
  //checking modbus topic or not
  bool Modbridge = false;
  char * modPacket;
  modPacket = strstr (topic, "mod");
  if (modPacket != NULL) {
    Modbridge = true;
  }

  if (Modbridge) { //if modbus packet treat payload as byte else treat as json string
    //-----------for modbus Serial Communication  -----
    byte Mod_msg[length]; //addition 2 byte is for CRC checksum

    //capuring payload data  byte capture and transfer
    for (int i = 0; i < length; i++) {
      Mod_msg[i] = (byte)payload[i];
    }
    DEBUG_PRINTLN();
    ModSerial.write(Mod_msg, length); //sending bytes to modbus serially
    delay(1);                       //
  } else {
#endif USE_MODBUS     
    //-----------for string capturing-----
    char message[length + 1];

    for (int i = 0; i < length; i++) {
      message[i] = (char)payload[i];
    }
    message[length] = '\0';
    DEBUG_PRINTLN(message);

    processJson(message);
#ifdef USE_MODBUS   
  }
#endif USE_MODBUS   
  /* MQTT COMMAND MANUAL
    ----------------------------------------------------------------------------------------------
    CURRENT_Module   commands          |                          |
    "DEVICE_IP" : "?"                  |                          |"Alert_tm":XXXX
    "UNIX_TIME":XXXXXXXX               |"MQTT_TOPIC" : "?"        |"Alert_tm":"?"
    "UNIX_TIME":"?"                    |"CompanyNm" : "xxxx"      |"Delta_Curr":XXXX
                                       |"DepartNm" : "xxxx"       |"Delta_Curr":"?"
                                       |"DeviceNo" : "xx"         |"Delta_tm":XXXX
                                       |"DeviceRst" : "any_thing" |"Delta_tm":"?"
    --------------------------------------------------------------------------------------------------
  */

}


bool processJson(char* message) {

  DynamicJsonDocument  JSONencoder(250); //for sending PACKET to MQTT

  StaticJsonDocument<256> doc; //for parse MQTT receiving packet
  DeserializationError error = deserializeJson(doc, message);

  if (error) {
    Serial1.println("parseObject() failed");
    return false;
  }

  //------------------CURRENT_MODULE----------------

  if (doc.containsKey("Alert_tm")) {
    if (doc["Alert_tm"] == get_cmd) {
      JSONencoder["Alert_tm"] = running_val.Alert_tm;
    } else {
      running_val.Alert_tm = int(doc["Alert_tm"]);
      EE.eeprom_write_Int(_ADDR_Alert_tm, running_val.Alert_tm);
      JSONencoder["Alert_tm"] = EE.eeprom_read_Int(_ADDR_Alert_tm);
    }
  }

  if (doc.containsKey("Delta_Curr")) {
    if (doc["Delta_Curr"] == get_cmd) {
      JSONencoder["Delta_Curr"] = running_val.Delta_Curr;
    } else {
      running_val.Delta_Curr = int(doc["Delta_Curr"]);
      EE.eeprom_write_Int(_ADDR_Delta_Curr, running_val.Delta_Curr);
      JSONencoder["Delta_Curr"] = EE.eeprom_read_Int(_ADDR_Delta_Curr);
    }
  }

  if (doc.containsKey("Delta_tm")) {
    if (doc["Delta_tm"] == get_cmd) {
      JSONencoder["Delta_tm"] = running_val.Delta_tm;
    } else {
      running_val.Delta_tm  = int(doc["Delta_tm"]);
      if (running_val.Delta_tm < TIMER_INTERVAL) {
        running_val.Delta_tm = TIMER_INTERVAL; //validation check for minimum 100ms
      }
      EE.eeprom_write_Int(_ADDR_Delta_tm, running_val.Delta_tm);
      JSONencoder["Delta_tm"] = EE.eeprom_read_Int(_ADDR_Delta_tm);
    }
  }

  if (doc.containsKey("CT_Ratio")) {
    if (doc["CT_Ratio"] == get_cmd) {
      JSONencoder["CT_Ratio"] = running_val.CT_Ratio;
    } else {
      running_val.CT_Ratio  = int(doc["CT_Ratio"]);
      EE.eeprom_write_Int(_ADDR_CT_Ratio, running_val.CT_Ratio);
      JSONencoder["CT_Ratio"] = EE.eeprom_read_Int(_ADDR_CT_Ratio);
    }
  }

  
  if (doc.containsKey("No_Load_Curr")) {
     if (doc["No_Load_Curr"] == get_cmd) {
      JSONencoder["No_Load_Curr"] = running_val.no_load_current;
    } else {
      running_val.no_load_current  = int(doc["No_Load_Curr"]);
      EE.eeprom_write_Int(_ADDR_NO_LOAD_CURR, running_val.no_load_current);
      JSONencoder["No_Load_Curr"] = EE.eeprom_read_Int(_ADDR_NO_LOAD_CURR);
    }
  }
  //-----------------IP_QUERY-------------
  if (doc.containsKey("DEVICE_IP")) {
    if (doc["DEVICE_IP"] == get_cmd) {
      IPAddress ret =  Ethernet.localIP() ;
      JSONencoder["DEVICE_IP"] =  ret.toString();
      //     JSONencoder["DEVICE_IP"] =  Ethernet.localIP_tostr();
    }
  }
  //-----------------unix_time Query/Control-------------
  if (doc.containsKey("UNIX_TIME")) {
    if (doc["UNIX_TIME"] == get_cmd) {
      if (EE.HW_ERROR) {  //if ds3231 is not there then use internal rtc
//        JSONencoder["UNIX_TIME"] =  intl_rtc.getTime();
        JSONencoder["UNIX_TIME"] =  "RTC_Err";
      } else {
        JSONencoder["UNIX_TIME"] =  getEpoch();
      }

    } else {
      unsigned long tim_temp = doc["UNIX_TIME"];
      setEpoch(tim_temp);
      JSONencoder["UNIX_TIME"] = "SET";
    }
  }

  //-----------------mqtt-topic generator-------------
  if (doc.containsKey("CompanyNm")) {
    const char* comTemp = doc["CompanyNm"];
    running_val.CompanyNm = String(comTemp);
    EE.eeprom_write_String(_ADDR_CompanyNm, running_val.CompanyNm);
    JSONencoder["CompanyNm"] = EE.eeprom_read_String(_ADDR_CompanyNm);
  }

  if (doc.containsKey("DepartNm")) {
    const char* DepartTemp = doc["DepartNm"];
    running_val.DepartNm = String(DepartTemp);
    EE.eeprom_write_String(_ADDR_DepartNm, running_val.DepartNm);
    JSONencoder["DepartNm"] =  EE.eeprom_read_String(_ADDR_DepartNm);
  }

  if (doc.containsKey("DeviceNo")) {
    const char* DeviceTemp = doc["DeviceNo"];
    running_val.DeviceNo = String(DeviceTemp);
    EE.eeprom_write_String(_ADDR_DeviceNo, running_val.DeviceNo);
    JSONencoder["DeviceNo"] =  EE.eeprom_read_String(_ADDR_DeviceNo);
  }


  if (doc.containsKey("DeviceRst")) {

#ifdef  WATCHDOG_ENABLE
    iwdg_init(IWDG_PRE_256, 250 ); //watch-dog 250*0.0064 = 1600 ms
#endif//WATCH_ENABLE   

    while (1); //stuct here untill watchdog trigger restart
  }

  //-----------------Mqtt Topic Debug-------------
  if (doc.containsKey("MQTT_TOPIC")) {
    if (doc["MQTT_TOPIC"]     == get_cmd) {
      JSONencoder["SUB"]       = Sub_Topic;
      JSONencoder["PUB/OUT"]   = Pub_Topic_OUT;
      JSONencoder["PUB/RES"]   = Pub_Topic_RES;
      JSONencoder["PUB/BIRTH"] = Pub_Topic_BIRTH;
    }
  }

  //-----------------Modbus Configuration -------------
  if (doc.containsKey("MOD_CONFIG")) {
    if (doc["MOD_CONFIG"] == get_cmd) {
      JSONencoder["MOD_CONFIG"] = running_val.Mod_Config;
    } else {
      running_val.Mod_Config  = int(doc["MOD_CONFIG"]);
      EE.eeprom_write_Int(_ADDR_Mod_Config, running_val.Mod_Config);
      JSONencoder["MOD_CONFIG"] = EE.eeprom_read_Int(_ADDR_Mod_Config);
    }
  }

  if (doc.containsKey("MOD_BAUD")) {
    if (doc["MOD_BAUD"] == get_cmd) {
      JSONencoder["MOD_BAUD"] = running_val.Mod_Baud;
    } else {
      running_val.Mod_Baud  = int(doc["MOD_BAUD"]);
      EE.eeprom_write_Int(_ADDR_Mod_Baud, running_val.Mod_Baud);
      JSONencoder["MOD_BAUD"] = EE.eeprom_read_Int(_ADDR_Mod_Baud);
    }
  }
  //-------------------Setting the JSON Packet to cloud-----------
  if (JSONencoder.size() > 0) { // send online when payload contain any data
    //for sending over mqtt server
    char JSONmessageBuffer[250];
    serializeJson(JSONencoder, JSONmessageBuffer);
    mqttclient.publish(Pub_Topic_RES.c_str(), JSONmessageBuffer );
    mqttclient.publish(Pub_Topic_RES_DBG.c_str(), JSONmessageBuffer ); //for debugging via cloud
  }

  //  delay(50); //for UART Rx_buffer to be filled
  return true;
}

void mqtt_publish(Byte_type EEPROM_DATA, bool live_Status, bool alert_status) {
  digitalWrite(MQTT_PUSH_LED,LOW);
  //At no load condition current will be zero
  if (EEPROM_DATA.Irms <= ((float)running_val.no_load_current/1000) * ((float)running_val.CT_Ratio/1000)) { //at normal condition 0.05A which convert to 0.1
    EEPROM_DATA.Irms = 0;
  }
  
  DynamicJsonDocument  JSONencoder(250); //for sending PACKET to MQTT SERVER

  JSONencoder["unix_time"]    = EEPROM_DATA.unix_time ;
  JSONencoder["current"]      = EEPROM_DATA.Irms*1000;   //to convert Ampere to milliampere parameter * by 1000
  JSONencoder["periodic"]     = byte(!alert_status);
  JSONencoder["LIVE"]         = byte(live_Status);

  if (JSONencoder.size() > 0) { // send online when payload contain any data
    //for sending over mqtt server
    char JSONmessageBuffer[300];
    serializeJson(JSONencoder, JSONmessageBuffer);
    bool Publish_status = mqttclient.publish(Pub_Topic_OUT.c_str(), JSONmessageBuffer);

    Timer2.pause();
    if (!Publish_status) {
      EEPROM_WRITE(); //if publish failed dump to eeprom
    }
    Timer2.resume();

    DEBUG_PRINT("MQTT publish status:");
    DEBUG_PRINTLN(Publish_status ? "SUCCESS" : "FAILED");

  }

  reset_cnt = EEPROM_DATA.unix_time; //update eponch time to ram
}

#endif//MQTT_ENABLE
