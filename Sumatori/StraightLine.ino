void StraightLine(){
  GetWheelValues();
  if (rightWheelPosition < straightLineRadians*.85){
    MS1.setM2Brake(0);
    MS1.setM1Brake(0);
    MS1.setM1Speed(-leftStraightLineSpeed);
    MS1.setM2Speed(rightStraightLineSpeed);
    if (leftWheelVelocity > .01){
      leftStraightLineSpeed = int(250*rightWheelVelocity/leftWheelVelocity);
    }
  }
  else if (rightWheelPosition < straightLineRadians){
    MS1.setM2Brake(0);
    MS1.setM1Brake(0);
    MS1.setM1Speed(-75);
    MS1.setM2Speed(75);
    if (leftWheelVelocity > .01){
      leftStraightLineSpeed = int(75*rightWheelVelocity/leftWheelVelocity);
    }
  }
  else {
    MS1.setM2Brake(400);
    MS1.setM1Brake(400);
  }
}
