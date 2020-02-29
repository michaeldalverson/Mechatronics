void HallEffectPM8(){
  int timeElapsed = 0;
  int startTime = millis();
  while (millis() - startTime < 1500){
  Serial.println(analogRead(hallSensorIn));
  }
}
