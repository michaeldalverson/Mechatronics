void WallFollowing(){

  // Initializations
  uint16_t distanceValue = 0;
  uint16_t optimalDistanceVal = 300;
  uint8_t hysteresis = 30;

  // Read range finder val
  distanceValue = analogRead(rangeFinderIn);
  
  if (distanceValue > optimalDistanceVal - hysteresis && distanceValue < optimalDistanceVal + hysteresis){
    MS1.setM2Speed(0);
    MS1.setM1Speed(0);
  }
  else if (distanceValue < 50){
    MS1.setM1Speed(0);
    MS1.setM2Speed(0);
  }
  else if (distanceValue < optimalDistanceVal){
    MS1.setM1Speed(200);
    MS1.setM2Speed(200);
  }
  else {
    MS1.setM1Speed(-200);
    MS1.setM2Speed(-200);
  }
  return;
}
