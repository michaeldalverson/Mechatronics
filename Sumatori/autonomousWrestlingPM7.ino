void autonomousWrestlingPM7(){
  qtr.read(reflectSensorValues);
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
  return;
}
