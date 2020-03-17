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
    leftStraightLineSpeed = 75;
    rightStraightLineSpeed = 75;
    MS1.setM2Brake(0);
    MS1.setM1Brake(0);
    MS1.setM1Speed(-leftStraightLineSpeed);
    MS1.setM2Speed(rightStraightLineSpeed);
    if (leftWheelVelocity > .01){
      leftStraightLineSpeed = int(75*rightWheelVelocity/leftWheelVelocity);
    }
  }
  else {
    StopCommand();
  }
}
