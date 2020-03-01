void StopCommand(){
    #ifdef debug
      Serial.println("Stop Received");
    #endif
    teleoperatedFlag = false;
    stopFlag = true;
    entranceFlag = false;
    autonomousFlag = false;

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
