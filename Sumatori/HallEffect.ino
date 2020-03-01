void HallEffect(){
  
  while (true){
    // Read vals
    hallSensorVal = analogRead(hallSensorIn);
    #ifdef sensor
      Serial.print("Hall: ");
      Serial.println(hallSensorVal);
    #endif

    // If less than 1.5 inches away, break
    if (hallSensorVal > 960 || hallSensorVal < 60){
      #ifdef debug
        Serial.println("Magnet Detected!");
      #endif
      break;
    }

    // Empty buffer and check for S and delay a bit
    if (Serial3.read() == "S"){
      StopCommand();
    }
    delay(10);
  }
}
