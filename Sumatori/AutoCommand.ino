void AutoCommand() {
#ifdef debug
  Serial.println("Autonomous Received");
#endif
  teleoperatedFlag = false;
  stopFlag = false;
  entranceFlag = false;
  autonomousFlag = true;

  // Turn off all brakes
  MS1.setM1Brake(0);
  MS1.setM2Brake(0);
  MS2.setM1Brake(0);
  MS2.setM2Brake(0);
}

