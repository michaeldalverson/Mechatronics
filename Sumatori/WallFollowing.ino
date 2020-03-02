bool WallFollowing(){
  delay(2);

  // Read range finder val
  distanceValue = int(((1-rangeFinderAlpha)*distanceValue) + (rangeFinderAlpha*analogRead(rangeFinderIn)));
  
  // Debugging purposes
  #ifdef sensor
    Serial.print("Range Finder Value: ");
    Serial.println(distanceValue);
  #endif

  // If distance is outside the threshold, return true to end wall following
  if (distanceValue < rangeThreshold){
    return false;
  }
  // If wall is optimal distance, move straight
  else if (distanceValue >= minRangeVal && distanceValue <= maxRangeVal){
    #ifdef debug
      Serial.println("Move Straight");
    #endif
    MS1.setM1Speed(300);
    MS1.setM2Speed(300);
    MS1.setM2Brake(0);
    MS1.setM1Brake(0);
  }
  // If wall is too far, move right
  else if (distanceValue < minRangeVal){
    #ifdef debug
      Serial.println("Move Right");
    #endif
    MS1.setM2Brake(0);
    MS1.setM1Brake(0);
    MS1.setM1Speed(-250);
    MS1.setM2Speed(150);
  }
  // If wall is too close, move left
  else {
    #ifdef debug
      Serial.println("Move Left");
    #endif
    MS1.setM2Brake(0);
    MS1.setM1Brake(0);
    MS1.setM1Speed(250);
    MS1.setM2Speed(-150);
  }
  return true;
}

