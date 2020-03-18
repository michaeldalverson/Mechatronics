void RightTurn() {

  // Get position and velocity of wheels
  GetWheelValues();
  leftWheelPIDTurn.Compute();
  rightWheelPIDTurn.Compute();

#ifdef MOTORSPEED
  Serial.print("L: ");
  Serial.print(leftWheelSpeedStraight);
  Serial.print(" R: ");
  Serial.println(rightWheelSpeedStraight);
#endif

  // Set right wheel speed
  if (leftWheelPosition < rightTurnRadiansLeft) {
    MS1.setM1Brake(0);
    MS1.setM1Speed(-leftWheelSpeedTurn);
  }
  else {
    MS1.setM1Speed(0);
    MS1.setM1Brake(100);
  }
  // Set left wheel speed
  if (rightWheelPosition < rightTurnRadiansRight) {
    MS1.setM2Brake(0);
    if (rightWheelVelocity < .05 && leftWheelVelocity < .05) {
      MS1.setM2Speed(rightWheelSpeedTurn);
      modMotorTurning = rightWheelSpeedTurn;
    }
    else {
      modMotorTurning =  modMotorTurning * pow(leftWheelVelocity * wheelTurnProportion / rightWheelVelocity, 2);
      Serial.println(modMotorTurning);
      MS1.setM2Speed(constrain(modMotorTurning, 0, 400));
    }
  }
  else {
    MS1.setM2Speed(0);
    MS1.setM2Brake(100);
  }
  if (rightWheelPosition >= rightTurnRadiansRight && leftWheelPosition >= rightTurnRadiansLeft) {
    // Stop robot once destination reached
    StopCommand();
  }

  
  /*
  GetWheelValues();
  if (leftWheelPosition < rightTurnRadiansLeft || rightWheelPosition < rightTurnRadiansRight) {
    MS1.setM2Brake(0);
    MS1.setM1Brake(0);
    MS1.setM1Speed(-leftTurnSpeed);
    MS1.setM2Speed(rightTurnSpeed);
    if (rightWheelVelocity > .025 && leftWheelVelocity > .025) {
      rightTurnSpeed = int(leftTurnSpeed * wheelTurnProportion * wheelTurnProportion * leftWheelVelocity / rightWheelVelocity);
    }
#ifdef debug
    Serial.print("R Speed: ");
    Serial.print(rightTurnSpeed);
    Serial.print(" L Speed: ");
    Serial.println(leftTurnSpeed);
#endif
  }
  else {
    StopCommand();
  }
  if (leftWheelPosition > rightTurnRadiansLeft &&  rightTurnRadiansRight < .05) {
    StopCommand();
  }*/
}
