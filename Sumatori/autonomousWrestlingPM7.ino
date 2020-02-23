void autonomousWrestlingPM7(){

  // Read qtr values
  qtr.read(reflectSensorValues);

  // Initializations
  int16_t biasedSensorValues[reflectSensorCount];
  double lineLocNum = 0;
  double lineLocDen = 0;

  for (unsigned char i = 0; i < reflectSensorCount;i++){
    biasedSensorValues[i] = reflectSensorValues[i] - biasArray[i];
    if (biasedSensorValues[i] < 0){
      biasedSensorValues[i] = 0;
    }
    lineLocNum = lineLocNum + ((i+1) * biasedSensorValues[i]);
    lineLocDen = lineLocDen + biasedSensorValues[i];
  }
  double lineLoc = lineLocNum/lineLocDen;
  if (lineLoc < -1*bound){
    MS1.setM2Speed(200);
    MS1.setM1Speed(0);
    
  }
  else if (lineLoc > bound){
    MS1.setM1Speed(200);
    MS1.setM2Speed(0);
  }
  else {
    MS1.setM1Speed(100);
    MS1.setM2Speed(100);
  }
  return;
}
