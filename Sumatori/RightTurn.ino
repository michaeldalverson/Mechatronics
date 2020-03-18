void RightTurn() {
  
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
  }
}
