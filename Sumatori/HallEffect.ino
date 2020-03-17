bool HallEffect() {

  // Read vals with IIR
  hallSensorVal = int(((1 - hallSensorAlpha) * hallSensorVal) + (hallSensorAlpha * analogRead(hallSensorIn)));

#ifdef sensor
  Serial.print("Hall: ");
  Serial.println(hallSensorVal);
#endif

  // If less than 1.5 inches away return true
  if (hallSensorVal >= hallSensorMax || hallSensorVal <= hallSensorMin) {
#ifdef debug
    Serial.println("Magnet Detected!");
#endif
    return true;
  }

  // Else return false and continue checking
  else {
    return false;
  }
}

