void Teleoperation(){
  if (false){
    MS1.setM1Speed(leftStickSpeed);
  }

  if (false){
    MS1.setM2Speed(rightStickSpeed);
  }
  
  if(false){
    #ifdef debug
      Serial.println("Fork Up");
    #endif
    MS2.setM1Brake(0);
    MS2.setM1Speed(400);
  }

  if (false){
    #ifdef debug
      Serial.println("Fork Down");
    #endif
    MS2.setM1Brake(0);
    MS2.setM1Speed(-400);
  }

  if (false){
    #ifdef debug
      Serial.println("Fork Locked");
    #endif
    MS2.setM1Speed(0);
    MS2.setM1Brake(400);
  }

  if (false){
    MS2.setM2Speed(wiperSpeed);
  }
}
