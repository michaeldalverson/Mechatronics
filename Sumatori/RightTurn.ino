void RightTurn(){
  GetWheelValues();
  if (leftWheelPosition < rightTurnRadians){
    MS1.setM2Brake(0);
    MS1.setM1Brake(0);
    MS1.setM1Speed(-leftTurnSpeed);
    MS1.setM2Speed(rightTurnSpeed);
  }
  else {
    MS1.setM2Brake(400);
    MS1.setM1Brake(400);
  }
}

