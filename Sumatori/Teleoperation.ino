void Teleoperation() {

  // Set Motor Speeds
  MS1.setM1Speed(leftStickSpeed);
  MS1.setM2Speed(rightStickSpeed);
  MS2.setM2Speed(wiperSpeed);

  // Debug
#ifdef debug
  Serial.print("Left Wheel: ");
  Serial.println(leftStickSpeed);
  Serial.print("Right Wheel: ");
  Serial.println(rightStickSpeed);
  Serial.print("Wiper: ");
  Serial.println(wiperSpeed);
#endif

  if (forkLiftVal == 'U') {
#ifdef debug
    Serial.println("Fork Up");
#endif
    MS2.setM1Brake(0);
    MS2.setM1Speed(400);
  }
  else if (forkLiftVal == 'D') {
#ifdef debug
    Serial.println("Fork Down");
#endif
    MS2.setM1Brake(0);
    MS2.setM1Speed(-400);
  }
  else if (forkLiftVal == 'F') {
#ifdef debug
    Serial.println("Fork Locked");
#endif
    MS2.setM1Speed(0);
    MS2.setM1Brake(400);
  }
}

