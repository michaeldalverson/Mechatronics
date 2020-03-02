void EntranceCommand(){   
  #ifdef debug
    Serial.println("Entrance Received");
  #endif
  teleoperatedFlag = false;
  stopFlag = false;
  entranceFlag = true;
  autonomousFlag = false;

  // Turn off all brakes
  MS1.setM1Brake(0);
  MS1.setM2Brake(0);
  MS2.setM1Brake(0);
  MS2.setM2Brake(0);
}
