void StraightLine() {

  // Get position and velocity of wheels
  GetWheelValues();
  leftWheelPIDStraight.Compute();
  rightWheelPIDStraight.Compute();

#ifdef MOTORSPEED
  Serial.print("L: ");
  Serial.print(leftWheelSpeedStraight);
  Serial.print(" R: ");
  Serial.println(rightWheelSpeedStraight);
#endif

  // Set right wheel speed
  if (leftWheelPosition < straightLineRadians) {
    MS1.setM1Brake(0);
    MS1.setM1Speed(-leftWheelSpeedStraight);
  }
  else {
    MS1.setM1Speed(0);
    MS1.setM1Brake(100);
  }
  // Set left wheel speed
  if (rightWheelPosition < straightLineRadians) {
    MS1.setM2Brake(0);
    if (rightWheelVelocity < .05 && leftWheelVelocity < .05) {
      MS1.setM2Speed(rightWheelSpeedStraight);
      modMotorVal = rightWheelSpeedStraight;
    }
    else {
      modMotorVal =  modMotorVal * pow(leftWheelVelocity / rightWheelVelocity, 2);
      Serial.println(modMotorVal);
      MS1.setM2Speed(constrain(modMotorVal, 0, 400));
    }
  }
  else {
    MS1.setM2Speed(0);
    MS1.setM2Brake(100);
  }
  if (rightWheelPosition >= straightLineRadians && leftWheelPosition >= straightLineRadians) {
    // Stop robot once destination reached
    StopCommand();
  }
  /*
    if (rightWheelPosition < straightLineRadians * .85) {
     MS1.setM2Brake(0);
     MS1.setM1Brake(0);
     MS1.setM1Speed(-leftStraightLineSpeed);
     MS1.setM2Speed(rightStraightLineSpeed);
     if (leftWheelVelocity > .01) {
       leftStraightLineSpeed = int(250 * rightWheelVelocity / leftWheelVelocity);
     }
    }
    else if (rightWheelPosition < straightLineRadians) {
     leftStraightLineSpeed = 75;
     rightStraightLineSpeed = 75;
     MS1.setM2Brake(0);
     MS1.setM1Brake(0);
     MS1.setM1Speed(-leftStraightLineSpeed);
     MS1.setM2Speed(rightStraightLineSpeed);
     if (leftWheelVelocity > .01) {
       leftStraightLineSpeed = int(75 * rightWheelVelocity / leftWheelVelocity);
     }
    }
    else {
     StopCommand();
    }
  */
}
