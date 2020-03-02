void ParseSerialComms() {
  // Read until beginning of packet is found
  if (Serial3.available()){
    // Read until start of buffer is received
    if (Serial3.read() == char(2)){
      
      #ifdef serial
        Serial.print("Packet Received: ");
      #endif

      // Read until packet is exausted or terminator breaks loop
      while (Serial3.available()){
        serialVal = Serial3.read();

        #ifdef serial
          Serial.print("2");
        #endif

        // If terminator is found, leave loop
        if (serialVal == char(4)){
          break;
        }
      }
      #ifdef serial
        Serial.println();
      #endif   
    }
  }
}
//    while (serialVal != char(3) || serialVal != char(4)){
//
//      // Read serial 
//      serialVal = Serial3.read();
//      #ifdef serial
//        Serial.print(serialVal);
//      #endif
//
//      // Skip empty vals
//      if (serialVal == char(-1)){
//        continue;
//      }
//    }
//    
//    while (serialVal != char(4)){
//      serialVal = Serial3.read();
//
//      #ifdef serial
//        Serial.print(serialVal);
//      #endif
//
//      if (serialVal == char(-1)){
//        continue;
//      }
//    }
//    serialVal = -1;
//    #ifdef serial
//      Serial.println();
//    #endif


