void LineFollowing(){

  // Read qtr values
  qtr.read(reflectSensorValues);

  // Debugging purposes
  #ifdef sensor
    for (unsigned char i = 0; i < reflectSensorCount;i++){
      Serial.print(reflectSensorValues[i]);
      Serial.print("\t");
    }
    Serial.println();
  #endif

  // Initializations
  int16_t biasedSensorValues[reflectSensorCount];
  double lineLocNum = 0;
  double lineLocDen = 0;
  double lineLoc = 0;

  // Read sensor values minus bias
  for (unsigned char i = 0; i < reflectSensorCount;i++){
    biasedSensorValues[i] = reflectSensorValues[i] - biasArray[i];
    if (biasedSensorValues[i] < 0){
      biasedSensorValues[i] = 0;
    }
    lineLocNum += ((i+1) * biasedSensorValues[i]);
    lineLocDen += biasedSensorValues[i];
  }

  // Get line location estimate
  lineLoc = lineLocNum/lineLocDen - 4.5;

  #ifdef debug
    Serial.print("Line location: ");
    Serial.println(lineLoc);
  #endif

  // Change motor speed depending on line location
  if (lineLoc < -2.5){
    MS1.setM2Speed(int(lineFollowingSpeedMultiplier*200));
    MS1.setM1Speed(int(lineFollowingSpeedMultiplier*200));
    delay(100);
    MS1.setM2Speed(int(lineFollowingSpeedMultiplier*200));
    MS1.setM1Speed(int(lineFollowingSpeedMultiplier*-200));
    delay(100);
  }
  else if (lineLoc > 2.5){
     MS1.setM2Speed(int(lineFollowingSpeedMultiplier*-200));
     MS1.setM1Speed(int(lineFollowingSpeedMultiplier*-200));
     delay(100);
     MS1.setM2Speed(int(lineFollowingSpeedMultiplier*200));
     MS1.setM1Speed(int(lineFollowingSpeedMultiplier*-200));
     delay(100);
  }
  else if (lineLoc < -bound){
    #ifdef debug
      Serial.print("Left L: ");
      Serial.print(-200);
      Serial.print(" R: ");
      Serial.println(400);
    #endif
    MS1.setM2Speed(int(lineFollowingSpeedMultiplier*300));
    MS1.setM1Speed(int(lineFollowingSpeedMultiplier*-100));
  }
  else if (lineLoc > bound){
    #ifdef debug
      Serial.print("Right L: ");
      Serial.print(-400);
      Serial.print(" R: ");
      Serial.println(200);
    #endif
    MS1.setM1Speed(int(lineFollowingSpeedMultiplier*-300));
    MS1.setM2Speed(int(lineFollowingSpeedMultiplier*100));
  }
  else { 
    // Note it is possible for lineLoc to be NaN (all sensors reading 0 after bias)
    // but it will just go straight if this happens
    #ifdef debug
      Serial.println("Straight");
    #endif
    MS1.setM1Speed(int(lineFollowingSpeedMultiplier*-200));
    MS1.setM2Speed(int(lineFollowingSpeedMultiplier*200));
  }
  
  return;
}

