void GetWheelValues(){
  delay(5);

  count++;

  // LEFT WHEEL CALCULATIONS
  startTimeLeft = millis();
  
  // Get pos and vel of gearbox
  leftWheelPosition = 2 * M_PI * leftEncoderCount / (gearRatio * countsPerRev);
  leftWheelVelocity = 1000 * (leftWheelPosition - leftWheelPositionOld) / (startTimeLeft - endTimeLeft);

  #ifdef sensor // Print positions and velocities for debugging
    if (count > 10){
      Serial.print("Left : P = ");
      Serial.print(leftWheelPosition);
      Serial.print(" V = ");
      Serial.print(leftWheelVelocity);
      Serial.println(); 
    }
  #endif

  // Store data for derivs
  leftWheelPositionOld = leftWheelPosition;
  endTimeLeft = startTimeLeft;


  // RIGHT WHEEL CALCULATIONS
  startTimeRight = millis();
  
  // Get pos and vel of gearbox
  rightWheelPosition = 2 * M_PI * rightEncoderCount / (gearRatio * countsPerRev);
  rightWheelVelocity = 1000 * (rightWheelPosition - rightWheelPositionOld) / (startTimeRight - endTimeRight);

  #ifdef sensor // Print positions and velocities for debugging
    if (count > 10){
      Serial.print("Right: P = ");
      Serial.print(rightWheelPosition);
      Serial.print(" V = ");
      Serial.print(rightWheelVelocity);
      Serial.println(); 
      count = 0;
    }
  #endif

  // Store data for derivs
  rightWheelPositionOld = rightWheelPosition;
  endTimeRight = startTimeRight;
  
}
