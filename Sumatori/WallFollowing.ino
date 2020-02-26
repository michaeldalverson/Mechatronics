void WallFollowing(){
  delay(2);

  // Read range finder val
  distanceValue = int(((1-rangeFinderAlpha)*distanceValue) + (rangeFinderAlpha*analogRead(rangeFinderIn)));
  
  // Debugging purposes
  #ifdef sensor
    Serial.print("Range Finder Value: ");
    Serial.println(distanceValue);
  #endif

  // Change motor speed depending on value
  if (distanceValue >= minRangeVal && distanceValue <= maxRangeVal){
    #ifdef debug
      Serial.println("No Movement");
    #endif
    MS1.setM2Speed(0);
    MS1.setM1Speed(0);
    MS1.setM2Brake(400);
    MS1.setM1Brake(400);
  }
  else if (distanceValue < 50){
    #ifdef debug
      Serial.println("No Movement");
    #endif
    MS1.setM1Speed(0);
    MS1.setM2Speed(0);
    MS1.setM2Brake(400);
    MS1.setM1Brake(400);
  }
  else if (distanceValue < minRangeVal){
    #ifdef debug
      Serial.println("Forward");
    #endif
    MS1.setM2Brake(0);
    MS1.setM1Brake(0);
    MS1.setM1Speed(-200);
    MS1.setM2Speed(200);
  }
  else {
    #ifdef debug
      Serial.println("Reverse");
    #endif
    MS1.setM2Brake(0);
    MS1.setM1Brake(0);
    MS1.setM1Speed(200);
    MS1.setM2Speed(-200);
  }
  
  return;
}
