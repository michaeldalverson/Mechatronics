#include <SoftwareSerial.h>

//#define inputs // Uncomment to print all input values from controller components
#define outputs // Uncomment to print all sent serial values

//Serial Communication
SoftwareSerial controllerSerial(2,3);
const uint8_t serialWriteLength = 15;
char serialWriteValues[serialWriteLength] = {'-'};
byte serialWriteLocation = 0;

//Analog Pins
//Wheel control sticks
const int leftStickVertIn = A0;
const int rightStickVertIn = A1;
const int tempWiperControl = A2;

//Digital Pins
//Master control LEDS
const int stopAllLEDOut = 4;
const int ceremonyLEDOut = 5;
const int teleLEDOut = 6;
const int autoLEDOut = 7;

//Master control buttons
const int stopAllIn = 8;
const int ceremonyIn = 9;
const int teleIn = 10;
const int autoIn = 11;

//ForkLift Control
const int forkLiftRightUpIn = 13;
const int forkLiftLeftDownIn = 12;

// VALUES
bool buttonPressedT = false;
bool buttonPressedE = false;

//Buttons
bool entranceCeremony;
bool autonomousWrestling;
bool teleWrestling;
bool STOP;

// Stick Hysteresis
uint8_t stickHysteresis = 50; // Sends 0 stick values if -x+512 < stickVal < x+512
uint16_t minStickActivation = 512 - stickHysteresis;
uint16_t maxStickActivation = 512 + stickHysteresis;

//Left Wheel
uint16_t leftStickVal;
int8_t leftStickSpeed;
bool leftStickZero = true;
int zeroCountLeft = 0;

//Right Wheel
uint16_t rightStickVal;
int8_t rightStickSpeed;
bool rightStickZero = true;
int zeroCountRight = 0;

//Forklift
bool forkUp;
bool forkDown;
char forkState;
bool forkZero = true;

//Wiper 
uint16_t rightWipeHorizontal;
int8_t wipeVal;
bool wipeZero = true;
int zeroCountWipe = 0;
void setup() {
  
  // Controller sticks
  pinMode(leftStickVertIn, INPUT);
  pinMode(rightStickVertIn, INPUT);

  // Master control LEDS
  pinMode(stopAllLEDOut, OUTPUT);
  pinMode(ceremonyLEDOut, OUTPUT);
  pinMode(teleLEDOut, OUTPUT);
  pinMode(autoLEDOut, OUTPUT);

  // Master control buttons
  pinMode(stopAllIn, INPUT);
  pinMode(ceremonyIn, INPUT);
  pinMode(teleIn, INPUT);
  pinMode(autoIn, INPUT);

  // Forklift control
  pinMode(forkLiftRightUpIn, INPUT);
  pinMode(forkLiftLeftDownIn, INPUT);

  // Wiper
  pinMode(tempWiperControl,INPUT);
  
  // Serial
  controllerSerial.begin(9600);
  Serial.begin(9600);
}

void loop() {
  
  delay(30);
  
  // ***************************** READ INPUTS ******************************
  // Read Sticks
  leftStickVal = analogRead(leftStickVertIn);
  rightStickVal = analogRead(rightStickVertIn);
  rightWipeHorizontal = analogRead(tempWiperControl);

  // Read Forklift Vals
  forkUp = digitalRead(forkLiftRightUpIn);
  forkDown = digitalRead(forkLiftLeftDownIn);

  // Read input buttons
  entranceCeremony = digitalRead(ceremonyIn);
  teleWrestling = digitalRead(teleIn);
  autonomousWrestling = digitalRead(autoIn);
  STOP = digitalRead(stopAllIn);

  #ifdef inputs
    delay(1000);
    Serial.println("L  \tR  \tW  \tU\tD\tE\tT\tA\tS");
    Serial.print(leftStickVal);
    Serial.print("\t");
    Serial.print(rightStickVal);
    Serial.print("\t");
    Serial.print(rightWipeHorizontal);
    Serial.print("\t");
    Serial.print(forkUp);
    Serial.print("\t");
    Serial.print(forkDown);
    Serial.print("\t");
    Serial.print(entranceCeremony);
    Serial.print("\t");
    Serial.print(teleWrestling);
    Serial.print("\t");
    Serial.print(autonomousWrestling);
    Serial.print("\t");
    Serial.println(STOP);
  #endif

  // ***************************** PARSE INPUTS ******************************
  // Left stick
  if ((leftStickVal > minStickActivation) && (leftStickVal < maxStickActivation)) {
    leftStickSpeed = 0;
    leftStickZero = true;
  }
  else {
    leftStickSpeed = map(leftStickVal, 0, 1023, -127,127);
    leftStickZero = false;
  }

  //Right Stick 
  if ((rightStickVal > minStickActivation) && (rightStickVal < maxStickActivation)) {
    rightStickSpeed = 0;
    rightStickZero = true;
  }
  else {
    rightStickSpeed = map(rightStickVal, 0, 1023, 127,-127);
    rightStickZero = false;
  }
  
  //Temporary wiper code
  if ((rightWipeHorizontal > minStickActivation) && (rightWipeHorizontal < maxStickActivation)) {
    wipeVal = 0;
    wipeZero = true;
  }
  else {
    wipeVal = map(rightWipeHorizontal, 0, 1023, -127,127);
    wipeZero = false;
  }

  // Forklift
  if ((forkUp) && (!forkDown)) {
    Serial.println("Fork Up");
    forkZero = false;
    forkState = 'U';
  }
  else if ((forkDown) && (!forkUp)) {
    Serial.println("Fork Down");
    forkState = 'D';
    forkZero = false;
  }
  else if (!forkZero){
    Serial.println("Fork Locked");
    forkState = 'F';
    forkZero = true;
  }
  else {
    forkState = 'Z';
  }

  // ***************************** SETUP SERIAL WRITES ******************************
 
  // Write Left Stick if they are not zero
  if (!leftStickZero) {
    zeroCountLeft = 0;
    serialWriteValues[serialWriteLocation++] = 'L';
    serialWriteValues[serialWriteLocation++] = leftStickSpeed;
  }
  else if (zeroCountLeft <4){
    leftStickSpeed = 0;
    serialWriteValues[serialWriteLocation++] = 'L';
    serialWriteValues[serialWriteLocation++] = leftStickSpeed;
    zeroCountLeft++;
  }

  // Write Right Stick if they are not zero
  if (!rightStickZero) {
    serialWriteValues[serialWriteLocation++] = 'R';
    serialWriteValues[serialWriteLocation++] = rightStickSpeed;
    zeroCountRight = 0;
  }
  else if (zeroCountRight < 4){
    rightStickSpeed = 0;
    serialWriteValues[serialWriteLocation++] = 'R';
    serialWriteValues[serialWriteLocation++] = rightStickSpeed;
    zeroCountRight++;
  }

  // Temp Wiper write
  if (!wipeZero) {
    serialWriteValues[serialWriteLocation++] = 'W';
    serialWriteValues[serialWriteLocation++] = wipeVal;
    zeroCountWipe = 0;
  }
   else if (zeroCountWipe < 4){
    wipeVal = 0;
    serialWriteValues[serialWriteLocation++] = 'W';
    serialWriteValues[serialWriteLocation++] = wipeVal;
    zeroCountWipe++;
  }

  // Write Fork State
  if (forkState != 'Z'){
    serialWriteValues[serialWriteLocation++] = forkState;
  }

  // MAIN BUTTONS & LED CONTROL
  if (entranceCeremony){
    serialWriteValues[serialWriteLocation++] = 'E';
    digitalWrite(ceremonyLEDOut,HIGH);
    digitalWrite(stopAllLEDOut,LOW);
    digitalWrite(teleLEDOut,LOW);
    digitalWrite(autoLEDOut,LOW);
  }
  if (teleWrestling){
    serialWriteValues[serialWriteLocation++] = 'T';
    digitalWrite(ceremonyLEDOut,LOW);
    digitalWrite(stopAllLEDOut,LOW);
    digitalWrite(teleLEDOut,HIGH);
    digitalWrite(autoLEDOut,LOW);
  }
   if (autonomousWrestling){
    serialWriteValues[serialWriteLocation++] = 'A';
    digitalWrite(ceremonyLEDOut,LOW);
    digitalWrite(stopAllLEDOut,LOW);
    digitalWrite(teleLEDOut,LOW);
    digitalWrite(autoLEDOut,HIGH);
  }
   if (STOP){
    serialWriteValues[serialWriteLocation++] = 'S';
    digitalWrite(ceremonyLEDOut,LOW);
    digitalWrite(stopAllLEDOut,HIGH);
    digitalWrite(teleLEDOut,LOW);
    digitalWrite(autoLEDOut,LOW);
  }

  // ***************************** SERIAL WRITES ******************************
  if (serialWriteLocation > 0){
    serialWriteValues[serialWriteLength-1] = '*';
    #ifdef outputs
      Serial.println("Sending");
      for (int i = 0; i++; i<serialWriteLength){
        Serial.print(serialWriteValues[i]);
      }
      Serial.println();
    #endif
    Serial.write(serialWriteValues, serialWriteLength);
    serialWriteLocation = 0;
  }



  
}
