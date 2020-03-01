void ParseSerialComms {
  if Serial3.available(){
    Serial3.readBytesUntil('*', serialBufferArray, serialBufferSize)
  }

}

