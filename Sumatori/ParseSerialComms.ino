void ParseSerialComms() {
  // Read until beginning of packet is found
  if (Serial3.available()){
    // Read until start of buffer is received
    if (Serial3.read() == char(2)){

      bool midChar = false;
      bool buttonPressed = false;
      bool forkReceived = false;
      
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
            if (tempRightStickSpeed == 1) {
              rightStickSpeed = 0;
            }
            else{
              rightStickSpeed = map(tempRightStickSpeed, -127, 127, -400, 400);
            }
          }

          // Left Stick
          else if (serialVal == 'L'){
            delay(5);
            tempLeftStickSpeed = Serial3.read();
            if (tempLeftStickSpeed == 1) {
              leftStickSpeed = 0;
            }
            else{
              leftStickSpeed = map(tempLeftStickSpeed, -127, 127, 400, -400);
            }

          }

          // Wiper
          else if (serialVal == 'W'){
            delay(5);
            tempWiperSpeed = Serial3.read();
            if (tempWiperSpeed == 1) {
              wiperSpeed = 0;
            }
            else{
              wiperSpeed = map(tempWiperSpeed, -127, 127, -400, 400);
            }
          }
        }
        
        // Read button values
        else {
          if (serialVal == 'U') {
            forkLiftVal = 'U';
            forkReceived = true;
          }
          else if (serialVal == 'D') {
            forkLiftVal = 'D';
            forkReceived = true;
          }
          else if (serialVal == 'F') {
            forkLiftVal = 'F';
            forkReceived = true;
          }
          if (buttonPressed){
            continue;
          }
          else if (serialVal == 'E'){
            EntranceCommand();
            buttonPressed = true;
          }
          else if (serialVal == 'T'){
            TeleCommand();
            buttonPressed = true;
          }
          else if (serialVal == 'A'){
            AutoCommand();
            buttonPressed = true;
          }
          else if (serialVal == 'S'){
            StopCommand();
            buttonPressed = true;
          }
        }
      }

      if (!forkReceived){
        forkLiftVal = 'Z';
      }
      
      #ifdef serial
        Serial.println();
      #endif   
    }
  }
}




