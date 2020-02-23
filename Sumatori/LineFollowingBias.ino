
void LineFollowingBias(){

  // Local inits
  int timeElapsed = 0;
  int startTime = millis();
  uint8_t count = 0;
  uint16_t tempSensorVals[reflectSensorCount];
  uint32_t sumSensorVals[reflectSensorCount];

  // Set sum values equal to 0 at start
  for (int i = 0; i < reflectSensorCount; i++){
    sumSensorVals[i] = 0;
  }

  // Get the average biasing
  while(timeElapsed < 1000){
    // Get time elapsed
    timeElapsed = millis() - startTime;

    // Read sensor values
    qtr.read(tempSensorVals);

    // Increase counter
    count++;

    // Add sensor readings to sum
    for (int i = 0; i < reflectSensorCount; i++){
      sumSensorVals[i] += tempSensorVals[i];
    }

    // Time Delay
    delay(50);
  }

  // Average Bias Values
  for (int i = 0; i < reflectSensorCount; i++){
    biasArray[i] = sumSensorVals[i]/count;
  }
  
  return;
}
