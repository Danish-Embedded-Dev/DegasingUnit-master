#ifdef DEBUGOUT
void SetupSerialOut(){
  #ifdef  USE_SERIAL    
    Serial.begin(baudrate); 
  #endif//USE_SERIAL    
  #ifdef  USE_SERIAL1  
    Serial1.begin(baudrate); 
  #endif//USE_SERIAL1   
  #ifdef  USE_SERIAL2 
    Serial2.begin(baudrate); 
  #endif//USE_SERIAL2 
  #ifdef  USE_SERIAL3  
    Serial3.begin(baudrate); 
  #endif//USE_SERIAL3   
}
#endif//DEBUGOUT


#ifdef  I2C_ENABLE
void Setup_I2C(){
  Wire.begin();   //start I2C wire protocol for EEPROM and RTC
}
#endif//I2C_ENABLE

#ifdef USE_MODBUS
void serial_capture(byte *message,byte *max_lenght){ 
 byte message_pos = 0;
 while (ModSerial.available() > 0){ 
   //Read the next available byte in the serial receive buffer
   byte inByte = ModSerial.read();
   
   //Add the incoming byte to our message
   message[message_pos] = inByte;
   message_pos++; 
   delay(2);  //wait for UART to be filled 
 }
 *max_lenght = message_pos;
} 
#endif//USE_MODBUS
