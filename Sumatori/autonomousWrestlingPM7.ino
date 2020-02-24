void autonomousWrestlingPM7(){

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
    lineLocNum = lineLocNum + ((i+1) * biasedSensorValues[i]);
    lineLocDen = lineLocDen + biasedSensorValues[i];
  }

  // Get line location estimate
  lineLoc = lineLocNum/lineLocDen;

  // Change motor speed depending on line location
  if (lineLoc < -1*bound){
    #ifdef debug
      Serial.println("Left");
    #endif
    MS1.setM2Speed(200);
    MS1.setM1Speed(0);
  }
  else if (lineLoc > bound){
    #ifdef debug
      Serial.println("Right");
    #endif
    MS1.setM1Speed(200);
    MS1.setM2Speed(0);
  }
  else {
    #ifdef debug
      Serial.println("Straight");
    #endif
    MS1.setM1Speed(100);
    MS1.setM2Speed(100);
  }
  
  return;
}
