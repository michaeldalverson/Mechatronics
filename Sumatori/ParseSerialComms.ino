void ParseSerialComms() {
  // Read until beginning of packet is found
  if (Serial3.available()){
    // Read until start of buffer is received
    if (Serial3.read() == char(2)){

      bool midChar = false;
      #ifdef serial
        Serial.print("Packet Received: ");
      #endif

      delay(5);

      // Read until packet is exausted or terminator breaks loop
      while (Serial3.available()){
        delay(5);
        serialVal = Serial3.read();

        #ifdef serial
          Serial.print(serialVal);
        #endif

        // If terminator is found, leave loop
        if (serialVal == char(4)){
          break;
        }
        // Signifies end of stick values and start of button values
        else if (serialVal == char(3)){
          midChar = true;
          continue;
        }
        
        // Read stick values
        if (!midChar){

          // Right stick
          if (serialVal == 'R'){
            delay(5);
            tempRightStickSpeed = Serial3.read();
            rightStickSpeed = map(tempRightStickSpeed, -128, 127, -400, 400);
          }

          // Left Stick
          else if (serialVal == 'L'){
            delay(5);
            tempLeftStickSpeed = Serial3.read();
            leftStickSpeed = map(tempLeftStickSpeed, -128, 127, 400, -400);
          }

          // Wiper
          else if (serialVal == 'W'){
            delay(5);
            tempWiperSpeed = Serial3.read();
            wiperSpeed = map(tempWiperSpeed, -128, 127, -400, 400);
          }
        }
        
        // Read button values
        else {
          // OIJAOISJDJOAIJSDOIJASDOIJASDOIJASODIJASOIJD
        }
      }
      #ifdef serial
        Serial.println();
        
      #endif   
      #ifdef debug
        Serial.print("R: ");
        Serial.println(rightStickSpeed);
        Serial.print("L: ");
        Serial.println(leftStickSpeed);
        Serial.print("W: ");
        Serial.println(wiperSpeed);
      #endif
    }
  }
}



