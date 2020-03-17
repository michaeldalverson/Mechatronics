void TeleCommand() {
#ifdef debug
  Serial.println("Teleoperated Received");
#endif
  teleoperatedFlag = true;
  stopFlag = false;
  entranceFlag = false;
  autonomousFlag = false;

  // Turn motors off when teleop is pressed
  MS1.setM1Speed(0);
  MS1.setM2Speed(0);
  MS2.setM1Speed(0);
  MS2.setM2Speed(0);

  // Turn off all brakes
  MS1.setM1Brake(0);
  MS1.setM2Brake(0);
  MS2.setM1Brake(0);
  MS2.setM2Brake(0);

  // Set all controller variables to zero
  rightStickSpeed = 0;
  rightStickDirection = 0;
  tempRightStickSpeed = 0;
  leftStickSpeed = 0;
  tempLeftStickSpeed = 0;
  leftStickDirection = 0;
  forkLiftVal = 'Z';
  wiperSpeed = 0;
  tempWiperSpeed = 0;
}

