void WallFollowing(){

  // Initializations
  uint16_t distanceValue = 0;
  uint16_t optimalDistanceVal = 300;
  uint8_t hysteresis = 30;

  // Read range finder val
  distanceValue = analogRead(rangeFinderIn);

  // Debugging purposes
  #ifdef sensor
    Serial.print("Range Finder Value: ");
    Serial.println(distanceValue);
  #endif

  // Change motor speed depending on value
  if (distanceValue > optimalDistanceVal - hysteresis && distanceValue < optimalDistanceVal + hysteresis){
    #ifdef debug
      Serial.println("No Movement");
    #endif
    MS1.setM2Speed(0);
    MS1.setM1Speed(0);
  }
  else if (distanceValue < 50){
    #ifdef debug
      Serial.println("No Movement");
    #endif
    MS1.setM1Speed(0);
    MS1.setM2Speed(0);
  }
  else if (distanceValue < optimalDistanceVal){
    #ifdef debug
      Serial.println("Forward");
    #endif
    MS1.setM1Speed(200);
    MS1.setM2Speed(200);
  }
  else {
    #ifdef debug
      Serial.println("Reverse");
    #endif
    MS1.setM1Speed(-200);
    MS1.setM2Speed(-200);
  }
  
  return;
}
