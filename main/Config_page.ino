#ifdef  CONFIG_PAGE_ENABLE


String ReadString; //to store the get data from another End

void Local_config_mode() {

  Timer2.pause(); // pause timer2

  DEBUG_PRINTLN(F("Configuration_Mode"));

  // Create a client connection
  EthernetClient client = Local_server.available();
  if (client) {
    ReadString = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        //read char by char HTTP request
        if (ReadString.length() < 100) {
          ReadString += c;
        }

        //if HTTP request has ended
        if (c == '\n') {
          //---------------------------------------------------------------------------
          DEBUG_PRINTLN(ReadString); //see what captured

          //login page gatewaycheck
          if (KeyvalueSearch(ReadString, "username") == "admin" & KeyvalueSearch(ReadString, "pwd") == "extens1tyAdm1n") {
            login_page = false;
            web_Prev_tm = millis();
          }

          if (!login_page) { //check for the first page bypass
            if (ReadString.indexOf("CONFIG_CLOSE") > 1) {
              Update_Eeprom();
              login_page = true;
              break;
            }
            Update_Field(ReadString);
          }

          //now output HTML data header
          if (login_page) {
            Page_1(client); //call login_page
          } else {
            Page_2(client); //call config_page
          }

          delay(1);
          //stopping client
          client.stop();
          //-------------------------------------------------------------------
        }
      }
    }
  }

  DEBUG_PRINTLN("Configuration_Mode_closed");
}


void Page_1(EthernetClient client ) { //credential check page
  client.println(F("HTTP/1.1 200 OK"));
  client.println(F("Content-Type: text/html"));
  client.println();

  client.println(F("<HTML>"));
  client.println(F("<HEAD>"));
  client.println(F("<TITLE>Extensity </TITLE>"));
  client.println(F("<style> body{width: 129%;text-align: center; margin: 100px 0 0 -164px; background : #8DA6C8}h1{color: #3b3b73;}</style>"));
  client.println(F("</HEAD>"));

  client.println(F("<BODY>"));
  client.println(F("<H1>EXTENSITY</H1>"));
  client.println(F("<H2>ASSEMBLY LINE MONITOR</H2>"));

  client.println(F("<FORM ACTION='/' method=get >"));
  client.println(F("<form action=\"/action_page.php\">"));
  client.println(F("<label for='username'>Username:</label><br>"));
  client.println(F("<input type='text' id='username' name='username' value=''><br>"));
  client.println(F("<label for='pwd'>Password:</label><br>"));
  client.println(F("<input type='password' id='pwd' name='pwd' value=''><br><br>"));
  client.println(F("<input type=\'submit' value='Submit'> "));
  client.println(F("</form>"));

  client.println("");
  client.println(F("</BODY>"));
  client.println(F("</HTML>"));

  //---------END OF LOGIN_PAGE  ------------

}

void Page_2(EthernetClient client) { //----configuure page start----------
  client.println(F("HTTP/1.1 200 OK"));
  client.println(F("Content-Type: text/html"));
  client.println();

  client.println(F("<HTML>"));
  client.println(F("<HEAD>"));
  client.println(F("<TITLE>Extensity</TITLE>"));
  client.println(F("<style> body{width: 129%;text-align: center; margin: 100px 0 0 -164px; background : #8DA6C8 }h1{color: #3b3b73;}</style>"));
  client.println(F("</HEAD>"));

  client.println(F("<BODY>"));
  client.println(F("<H1>EXTENSITY</H1>"));
  client.println(F("<H2>ASSEMBLY LINE MONITOR</H2>"));


  client.println(F("<FORM ACTION='/' method=get >")); //uses NTP SERVER of web page
  client.print("NTP SERVER______________: <INPUT TYPE=TEXT NAME='NTP_SERVER' VALUE='"); client.print(String(running_val.NTPserver));
  client.println("' SIZE='25' MAXLENGTH='50'>");
  client.println(F("<INPUT   TYPE=SUBMIT NAME='submit' VALUE='Submit'>"));
  client.println(F("</FORM>"));

  client.println(F("<FORM ACTION='/' method=get >")); //uses MQTT SERVER of web page
  client.print("MQTT SERVER____________: <INPUT TYPE=TEXT NAME='MQTT_SERVER' VALUE='"); client.print(String(running_val.mqtt_server));
  client.println("' SIZE='25' MAXLENGTH='50'>");
  client.println(F("<INPUT   TYPE=SUBMIT NAME='submit' VALUE='Submit'>"));
  client.println(F("</FORM>"));

  client.println(F("<FORM ACTION='/' method=get >")); //uses IP/port of web page
  client.print("MQTT PORT_______________: <INPUT TYPE=TEXT NAME='MQTT_PORT' VALUE='"); client.print(String(running_val.mqtt_port));
  client.println("' SIZE='25' MAXLENGTH='50'>");
  client.println(F("<INPUT   TYPE=SUBMIT NAME='submit' VALUE='Submit'>"));
  client.println(F("</FORM>"));

  client.println(F("<FORM ACTION='/' method=get >")); //uses UDP HOST IP of web page
  client.print("UDP HOST IP______________: <INPUT TYPE=TEXT NAME='UDP_HOST_IP' VALUE='"); client.print(String(running_val.Udp_host_ip));
  client.println("' SIZE='25' MAXLENGTH='50'>");
  client.println(F("<INPUT   TYPE=SUBMIT NAME='submit' VALUE='Submit'>"));
  client.println(F("</FORM>"));
  
  client.println(F("<FORM ACTION='/' method=get >")); //uses UDP HOST PORT of web page
  client.print("UDP HOST PORT___________: <INPUT TYPE=TEXT NAME='UDP_HOST_PORT' VALUE='"); client.print(String(running_val.Udp_host_port));
  client.println("' SIZE='25' MAXLENGTH='50'>");
  client.println(F("<INPUT   TYPE=SUBMIT NAME='submit' VALUE='Submit'>"));
  client.println(F("</FORM>"));
  
  client.println(F("<FORM ACTION='/' method=get >")); //uses COMPANY NAME of web page
  client.print("COMPANY NAME__________: <INPUT TYPE=TEXT NAME='CompanyNm' VALUE='"); client.print(String(running_val.CompanyNm));
  client.println("' SIZE='25' MAXLENGTH='50'>");
  client.println(F("<INPUT   TYPE=SUBMIT NAME='submit' VALUE='Submit'>"));
  client.println(F("</FORM>"));

  client.println(F("<FORM ACTION='/' method=get >")); //uses DEPARTMENT NAME of web page
  client.print("DEPARTMENT_NAME______: <INPUT TYPE=TEXT NAME='DepartNm' VALUE='"); client.print(String(running_val.DepartNm));
  client.println("' SIZE='25' MAXLENGTH='50'>");
  client.println(F("<INPUT   TYPE=SUBMIT NAME='submit' VALUE='Submit'>"));
  client.println(F("</FORM>"));

  client.println(F("<FORM ACTION='/' method=get >")); //uses IP/port of web page
  client.print("DEVICE_ID________________: <INPUT TYPE=TEXT NAME='DeviceID' VALUE='"); client.print(String(running_val.DeviceID));
  client.println("' SIZE='25' MAXLENGTH='50' readonly>");
  client.println(F("<INPUT   TYPE=SUBMIT NAME='submit' VALUE='Submit'>"));
  client.println(F("</FORM>"));

  client.println(F("<FORM ACTION='/' method=get >")); //uses IP/port of web page
  client.print("DEVICE_NO._______________: <INPUT TYPE=TEXT NAME='DeviceNo' VALUE='"); client.print(String(running_val.DeviceNo));
  client.println("' SIZE='25' MAXLENGTH='50'>");
  client.println(F("<INPUT   TYPE=SUBMIT NAME='submit' VALUE='Submit'>"));
  client.println(F("</FORM>"));

  client.println(F("<FORM ACTION='/' method=get >")); //uses ALERT TIME of web page
  client.print("ALERT TIME_______________: <INPUT TYPE=TEXT NAME='Alert_tm' VALUE='"); client.print(String(running_val.Alert_tm));
  client.println("' SIZE='25' MAXLENGTH='50'>");
  client.println(F("<INPUT   TYPE=SUBMIT NAME='submit' VALUE='Submit'>"));
  client.println(F("</FORM>"));

  client.println(F("<FORM ACTION='/' method=get >")); //uses DELTA CURRENT of web page
  client.print("DELTA CURRENT___________: <INPUT TYPE=TEXT NAME='Delta_Curr' VALUE='"); client.print(String(running_val.Delta_Curr));
  client.println("' SIZE='25' MAXLENGTH='50'>");
  client.println(F("<INPUT   TYPE=SUBMIT NAME='submit' VALUE='Submit'>"));
  client.println(F("</FORM>"));

  client.println(F("<FORM ACTION='/' method=get >")); //uses DELTA TIME of web page
  client.print("DELTA TIME________________: <INPUT TYPE=TEXT NAME='Delta_tm' VALUE='"); client.print(String(running_val.Delta_tm));
  client.println("' SIZE='25' MAXLENGTH='50'>");
  client.println(F("<INPUT   TYPE=SUBMIT NAME='submit' VALUE='Submit'>"));
  client.println(F("</FORM>"));

  client.println(F("<FORM ACTION='/' method=get >")); //uses CT-RATIO of web page
  client.print("CT_Ratio(multiple of thousandth): <INPUT TYPE=TEXT NAME='CT_Ratio' VALUE='"); client.print(String(running_val.CT_Ratio));
  client.println("' SIZE='25' MAXLENGTH='50'>");
  client.println(F("<INPUT   TYPE=SUBMIT NAME='submit' VALUE='Submit'>"));
  client.println(F("</FORM>"));

  client.println(F("<FORM ACTION='/' method=get >")); //uses NO LOAD CURRENT of web page
  client.print("No_Load_Current_____________: <INPUT TYPE=TEXT NAME='NO_LOAD_CURRENT' VALUE='"); client.print(String(running_val.no_load_current));
  client.println("' SIZE='25' MAXLENGTH='50'>");
  client.println(F("<INPUT   TYPE=SUBMIT NAME='submit' VALUE='Submit'>"));
  client.println(F("</FORM>"));
  
  client.println(F("<FORM ACTION='/' method=get >")); 
  client.println(F("CONFIGURE DEFAULT_______: <INPUT TYPE=TEXT NAME='CONFIG_DEFAULT' VALUE='DEFAULT' SIZE='25' MAXLENGTH='50'>"));
  client.println(F("<INPUT TYPE = SUBMIT NAME='submit' VALUE='Submit'>"));
  client.println(F("</FORM>"));

  client.println(F("<FORM ACTION='/' method=get >"));  
  client.println(F("SAVE CONFIGURE & CLOSE: <INPUT TYPE=TEXT NAME='CONFIG_CLOSE' VALUE='YES' SIZE='25' MAXLENGTH='50'>"));
  client.println(F("<INPUT TYPE = SUBMIT NAME='submit' VALUE='Submit'>"));
  client.println(F("</FORM>"));

  client.println("");

  client.println(F("</BODY>"));
  client.println(F("</HTML>"));
  //----configuure page end----------
}


void Update_Field(String ReadString) {

  if (ReadString.indexOf("NTP_SERVER") > 1) {
    String temp = KeyvalueSearch(ReadString, "NTP_SERVER");
    if (temp.length() <= (_ADDR_MQTT_Server - _ADDR_NTP_Server - 2)) { //if input is greater then giving bytes it reject
      running_val.NTPserver = temp;
    }
  }

  if (ReadString.indexOf("MQTT_SERVER") > 1) {
    String temp = KeyvalueSearch(ReadString, "MQTT_SERVER");
    if (temp.length() <= (_ADDR_Port - _ADDR_MQTT_Server - 2)) { //-2 for checksum and starting address byte
      running_val.mqtt_server  = temp;
    }
  }

  if (ReadString.indexOf("MQTT_PORT") > 1) {
    String temp = KeyvalueSearch(ReadString, "MQTT_PORT");
    if (temp.length() <= (_ADDR_CompanyNm  - _ADDR_Port)) {
      running_val.mqtt_port = temp.toInt();
    }
  }
  
  if (ReadString.indexOf("UDP_HOST_IP") > 1) {
    String temp = KeyvalueSearch(ReadString, "UDP_HOST_IP");
    if (temp.length() <= (_ADDR_UDP_HOST_PORT - _ADDR_UDP_HOST_IP - 2)) { //-2 for checksum and starting address byte
      running_val.Udp_host_ip  = temp;
    }
  }

  if (ReadString.indexOf("UDP_HOST_PORT") > 1) {
    String temp = KeyvalueSearch(ReadString, "UDP_HOST_PORT");
    if (temp.length() <= (_ADDR_NO_LOAD_CURR  - _ADDR_UDP_HOST_PORT)) {
      running_val.Udp_host_port = temp.toInt();
    }
  }
  
  
  if (ReadString.indexOf("CompanyNm") > 1) {
    String temp = KeyvalueSearch(ReadString, "CompanyNm");
    if (temp.length() <= (_ADDR_DepartNm - _ADDR_CompanyNm - 2)) {
      running_val.CompanyNm = temp;
    }
  }

  if (ReadString.indexOf("DepartNm") > 1) {
    String temp = KeyvalueSearch(ReadString, "DepartNm");
    if (temp.length() <= (_ADDR_DeviceNo - _ADDR_DepartNm - 2 )) {
      running_val.DepartNm = temp;
    }
  }

  if (ReadString.indexOf("DeviceNo") > 1) {
    String temp = KeyvalueSearch(ReadString, "DeviceNo");
    if (temp.length() <= (_ADDR_Alert_tm - _ADDR_DeviceNo - 2)) {
      running_val.DeviceNo = temp;
    }
  }

  if (ReadString.indexOf("Alert_tm") > 1) {
    String temp = KeyvalueSearch(ReadString, "Alert_tm");
    if (temp.length() <= (_ADDR_Delta_Curr - _ADDR_Alert_tm )) {
      running_val.Alert_tm = temp.toInt();
    }
  }

  if (ReadString.indexOf("Delta_Curr") > 1) {
    String temp = KeyvalueSearch(ReadString, "Delta_Curr");
    if (temp.length() <= (_ADDR_Delta_tm - _ADDR_Delta_Curr )) {
      running_val.Delta_Curr = temp.toInt();
    }
  }

  if (ReadString.indexOf("Delta_tm") > 1) {
    String temp = KeyvalueSearch(ReadString, "Delta_tm");
    if (temp.length() <= (_ADDR_CT_Ratio - _ADDR_Delta_tm )) { 
      if (temp.toInt() < TIMER_INTERVAL) {
       running_val.Delta_tm = TIMER_INTERVAL; //validation check for minimum 100ms
      }else{ 
       running_val.Delta_tm = temp.toInt();
      }
    }
  }

  if (ReadString.indexOf("CT_Ratio") > 1) {
    String temp = KeyvalueSearch(ReadString, "CT_Ratio");
    if (temp.length() <= (_ADDR_Mod_Config - _ADDR_CT_Ratio )) {
      running_val.CT_Ratio = temp.toInt();
    }
  }

  if (ReadString.indexOf("NO_LOAD_CURRENT") > 1) {
    String temp = KeyvalueSearch(ReadString, "NO_LOAD_CURRENT");
    if (temp.length() <= (Reserved_pvt_bytes - _ADDR_NO_LOAD_CURR  )) {
      running_val.no_load_current = temp.toInt();
    }
  }
 

  if (ReadString.indexOf("CONFIG_DEFAULT") > 1) {
    memcpy(&running_val, &default_val, sizeof default_val); //copy default values to running parameter
  }

}

void Update_Eeprom() {

  EE.eeprom_write_String(_ADDR_NTP_Server , running_val.NTPserver);

  EE.eeprom_write_String(_ADDR_MQTT_Server , running_val.mqtt_server);

  EE.eeprom_write_Int( _ADDR_Port, running_val.mqtt_port);
  
  EE.eeprom_write_String(_ADDR_UDP_HOST_IP, running_val.Udp_host_ip);
  
  EE.eeprom_write_Int( _ADDR_UDP_HOST_PORT, running_val.Udp_host_port);

  EE.eeprom_write_String(_ADDR_CompanyNm,running_val.CompanyNm);
  
  EE.eeprom_write_String( _ADDR_DepartNm, running_val.DepartNm );

  EE.eeprom_write_String(_ADDR_DeviceNo, running_val.DeviceNo);
 
  EE.eeprom_write_Int(_ADDR_Alert_tm, running_val.Alert_tm);

  EE.eeprom_write_Int(_ADDR_Delta_Curr, running_val.Delta_Curr);

  EE.eeprom_write_Int(_ADDR_Delta_tm, running_val.Delta_tm);

  EE.eeprom_write_Int(_ADDR_CT_Ratio, running_val.CT_Ratio);
   
  EE.eeprom_write_Int(_ADDR_NO_LOAD_CURR, running_val.no_load_current);
  
  DEBUG_PRINTLN("------EEPROM_Value--");
  DEBUG_PRINT("MQTT_SERVER:"); DEBUG_PRINTLN(EE.eeprom_read_String(_ADDR_MQTT_Server));
  DEBUG_PRINT("NTP_SERVER:"); DEBUG_PRINTLN(EE.eeprom_read_String(_ADDR_NTP_Server));
  DEBUG_PRINT("MQTT_PORT:"); DEBUG_PRINTLN(EE.eeprom_read_Int(_ADDR_Port));
  DEBUG_PRINT("UDP_HOST_IP:"); DEBUG_PRINTLN(EE.eeprom_read_String(_ADDR_UDP_HOST_IP));
  DEBUG_PRINT("UDP_HOST_PORT:"); DEBUG_PRINTLN(EE.eeprom_read_Int(_ADDR_UDP_HOST_PORT));
  DEBUG_PRINT("COM_NAME:"); DEBUG_PRINTLN(EE.eeprom_read_String(_ADDR_CompanyNm));
  DEBUG_PRINT("DEPART_NAME:"); DEBUG_PRINTLN(EE.eeprom_read_String(_ADDR_DepartNm));
  DEBUG_PRINT("DEVICE_NO:"); DEBUG_PRINTLN(EE.eeprom_read_String(_ADDR_DeviceNo));
  DEBUG_PRINT("DELTA_CURR:"); DEBUG_PRINTLN(EE.eeprom_read_String(_ADDR_Delta_Curr));
  DEBUG_PRINT("DELTA_TIME:"); DEBUG_PRINTLN(EE.eeprom_read_String(_ADDR_Delta_tm));
  DEBUG_PRINT("CT_RATIO:"); DEBUG_PRINTLN(EE.eeprom_read_String(_ADDR_CT_Ratio)); 
   DEBUG_PRINT("NO_LOAD_CURRENT:"); DEBUG_PRINTLN(EE.eeprom_read_String(_ADDR_NO_LOAD_CURR)); 
  
}


String KeyvalueSearch(String payload, String key) {
  String value;
  int start_index = payload.indexOf(key) + key.length() + 1;
  int last_index;

  if (payload.indexOf("&") > payload.indexOf(key)) {
    last_index = payload.indexOf("&") - 1;
  } else {
    last_index = payload.indexOf(" H") - 1;
  }

  return Scraping_string(payload, start_index, last_index);
}

String Scraping_string(String payload, byte start_pos, byte end_pos) {
  String Scrapt_data;

  while (start_pos != (end_pos + 1)) {
    Scrapt_data += payload[start_pos];
    start_pos++;
  }

  return Scrapt_data;
}

#endif//CONFIG_PAGE_ENABLE
