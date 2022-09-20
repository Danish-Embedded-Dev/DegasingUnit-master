#ifdef MODULE_CURRENT_EVENT

byte config_success = false;

void Config_IO() {
  
  pinMode(Network_LED, OUTPUT);
  pinMode(MQTT_PUSH_LED, OUTPUT);
  pinMode(Rst_pin, OUTPUT); //reset is use for WiznetModule
  
  pinMode(config_switch, INPUT_PULLUP);

  //wiznet reset is use on startup in case of noise lost their data
  digitalWrite(Rst_pin, LOW); //send low pulse for 500ms WiznetModule
  delay(500);
  digitalWrite(Rst_pin, HIGH); //send low pulse for 500ms WiznetModule
  delay(500);

  digitalWrite(Network_LED,HIGH); //turn network indicator off use this because we use pnp transistor
  digitalWrite(MQTT_PUSH_LED,HIGH); //turn packet push indicator off use this because we use pnp transistor
}

/*
   Setup timer for non blocking mqtt lost status
   interupt generate for every define tm_interval(param) ms
*/
void Enable_Timer_Interrupt(int tm_interval) {
  Timer2.setChannel1Mode(TIMER_OUTPUTCOMPARE); // setting Mode as Compare...
  Timer2.setPeriod(tm_interval * 1000); // in microseconds, after this period one Tick/Count generates...
  Timer2.setCompare1(1);                       // when value of Counter equals this value, an Interrupt is generated...
  Timer2.attachCompare1Interrupt(handler_input); // On interrupt, this fn gets called...
}

void handler_input() {
  master_tick++;
  
  if(master_tick==0){master_tick++;} //when overflow

  if (!Link_up) { //if physical link is not connected fast blink start
    if (master_tick % 3 == 0) {
      digitalWrite(Network_LED, !digitalRead(Network_LED));
    }
  }else if(!mqtt_connected) { //if mqtt-broker not connected slow blink start
    if (master_tick % 5 == 0) {
      digitalWrite(Network_LED, !digitalRead(Network_LED));
    }
  }
  
  if(PUSH_ENABLE){
    digitalWrite(MQTT_PUSH_LED,HIGH);
  }
  if(!digitalRead(MQTT_PUSH_LED)){
    PUSH_ENABLE = true;
  }

  if(EE.HW_ERROR){    //if eeprom is not there use internal rtc to get time
//    EEPROM_DATA.unix_time    = intl_rtc.getTime();;
    EEPROM_DATA.unix_time    = 0;
  }else{
    EEPROM_DATA.unix_time    = getEpoch();
  }
  EEPROM_DATA.Irms         = emon1.calcIrms(running_val.No_of_Samples);  // Calculate Irms only;
  EEPROM_DATA.Irms         = EEPROM_DATA.Irms*((float)running_val.CT_Ratio/1000);

  if (master_tick % (running_val.Alert_tm * 5) == 0) { //for periodic data checking
    if (mqtt_connected) {
      mqtt_packet_update =  true;
      udp_packet_update  =  true;
      alert_status       =  LOW;
      
    }
  }

  if (!(master_tick % (running_val.Delta_tm / TIMER_INTERVAL))) { //for alert data checking 
    if((EEPROM_DATA.Irms < ((float)running_val.no_load_current/1000) * ((float)running_val.CT_Ratio/1000) )&& forced_zero_packet){ //last zero packet
      mqtt_packet_update =  true;
      udp_packet_update  =  true;
      forced_zero_packet =  false;
      alert_status  =  HIGH;
    }
    double difference = abs(EEPROM_DATA.Irms - LastReading);
    
    if (difference  >= double(running_val.Delta_Curr)/1000) {
      ethClient.interupt_event = true;
      forced_zero_packet =  true;
      if (mqtt_connected) { //network push or eeprom dump
        mqtt_packet_update =  true;
        udp_packet_update  = true;
        alert_status  =  HIGH;
      } else {
        udp_packet_update  = true;
        EEPROM_WRITE();
      }
    }
    
    LastReading =  EEPROM_DATA.Irms;
  }

}


#endif//MODULE_CURRENT_EVENT
