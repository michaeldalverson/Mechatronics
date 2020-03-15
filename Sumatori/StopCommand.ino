void StopCommand(){
  #ifdef debug
    Serial.println("Stop Received");
  #endif
  teleoperatedFlag = false;
  stopFlag = true;
  entranceFlag = false;
  autonomousFlag = false;

  entranceStep = 0;
  lmEnc.write(0);
  rmEnc.write(0);
  leftWheelPosition = 0;
  rightWheelPosition = 0;
  leftWheelVelocity = 0;
  rightWheelVelocity = 0;

  // Turn motors off 
  MS1.setM1Speed(0);
  MS1.setM2Speed(0);
  MS2.setM1Speed(0);
  MS2.setM2Speed(0);

  // Turn on all brakes
  MS1.setM1Brake(400);
  MS1.setM2Brake(400);
  MS2.setM1Brake(400);
  MS2.setM2Brake(400);
}
