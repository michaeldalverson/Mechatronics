bool HallEffect(){
  
  while (true){
    // Read vals
    hallSensorVal = analogRead(hallSensorIn);
    #ifdef sensor
      Serial.print("Hall: ");
      Serial.println(hallSensorVal);
    #endif

    // If less than 1.5 inches away return true
    if (hallSensorVal > 960 || hallSensorVal < 60){
      #ifdef debug
        Serial.println("Magnet Detected!");
      #endif
      return true;
    }

    // Else return false and continue checking
    else{
      return false;
    }
  }
}
