#include <SoftwareSerial.h>
//Serial Communication

SoftwareSerial controllerSerial(2,3);

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
  // put your setup code here, to run once:
  //Controller sticks
  pinMode(leftStickVertIn, INPUT);
  pinMode(rightStickVertIn, INPUT);

  //Master control LEDS
  pinMode(stopAllLEDOut, OUTPUT);
  pinMode(ceremonyLEDOut, OUTPUT);
  pinMode(teleLEDOut, OUTPUT);
  pinMode(autoLEDOut, OUTPUT);

  //Master control buttons
  pinMode(stopAllIn, INPUT);
  pinMode(ceremonyIn, INPUT);
  pinMode(teleIn, INPUT);
  pinMode(autoIn, INPUT);

  //Forklift control
  pinMode(forkLiftRightUpIn, INPUT);
  pinMode(forkLiftLeftDownIn, INPUT);

  pinMode(tempWiperControl,INPUT);
  //Serial

  controllerSerial.begin(9600);
  Serial.begin(9600);
}

void loop() {
  // READ VALUES
 

  // Read Left Stick
  leftStickVal = analogRead(leftStickVertIn);
  if ((leftStickVal > minStickActivation) && (leftStickVal < maxStickActivation)) {
    leftStickSpeed = 0;
    leftStickZero = true;
  }
  else {
    leftStickSpeed = map(leftStickVal, 0, 1023, -127,127);
    leftStickZero = false;
  }


  //Read Right Stick
  rightStickVal = analogRead(rightStickVertIn);
  if ((rightStickVal > minStickActivation) && (rightStickVal < maxStickActivation)) {
    rightStickSpeed = 0;
    rightStickZero = true;
  }
  else {
    rightStickSpeed = map(rightStickVal, 0, 1023, 127,-127);
    rightStickZero = false;
  }


  // Read Forklift Vals
  forkUp = digitalRead(forkLiftRightUpIn);
  forkDown = digitalRead(forkLiftLeftDownIn);

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


  // Read input buttons
  entranceCeremony = digitalRead(ceremonyIn);
  teleWrestling = digitalRead(teleIn);
  autonomousWrestling = digitalRead(autoIn);
  STOP = digitalRead(stopAllIn);

 // ***************************** SERIAL WRITES ******************************
 // Write Left Stick
  if (!leftStickZero) {
    // Print left stick values if they are not zero
    Serial.print("L: ");
    Serial.println(leftStickSpeed);
    controllerSerial.write('L');
    delay(5);
    controllerSerial.write(leftStickSpeed);
    delay(5);
    zeroCountLeft = 0;
  }
  else if (zeroCountLeft <4){
    controllerSerial.write('L');
    delay(5);
    leftStickSpeed = 0;
    controllerSerial.write(leftStickSpeed);
    delay(5);
    zeroCountLeft +=1;
  }

  // Write Right Stick
  if (!rightStickZero) {
    // Print right stick values if they are not zero
    Serial.print("R: ");
    Serial.println(rightStickSpeed);
    controllerSerial.write('R');
    delay(5);
    controllerSerial.write(rightStickSpeed);    
    delay(5); 
    zeroCountRight = 0;
  }
   else if (zeroCountRight < 4){
    controllerSerial.write('R');
    delay(5);
    rightStickSpeed = 0;
    controllerSerial.write(rightStickSpeed);
    delay(5);
    zeroCountRight+=1;
  }

  // Write Fork State
  if (forkState != 'Z'){
    controllerSerial.write(forkState);
    delay(5);
  }

  // MAIN BUTTONS & LED CONTROL
  if (entranceCeremony){
    controllerSerial.write('E');
    delay(35);
    digitalWrite(ceremonyLEDOut,HIGH);
    digitalWrite(stopAllLEDOut,LOW);
    digitalWrite(teleLEDOut,LOW);
    digitalWrite(autoLEDOut,LOW);
  }
  if (teleWrestling){
    controllerSerial.write('T');
    delay(35);
     digitalWrite(ceremonyLEDOut,LOW);
    digitalWrite(stopAllLEDOut,LOW);
    digitalWrite(teleLEDOut,HIGH);
    digitalWrite(autoLEDOut,LOW);
  }
   if (autonomousWrestling){
    delay(35);
    controllerSerial.write('A');
     digitalWrite(ceremonyLEDOut,LOW);
    digitalWrite(stopAllLEDOut,LOW);
    digitalWrite(teleLEDOut,LOW);
    digitalWrite(autoLEDOut,HIGH);
  }
   if (STOP){
    delay(35);
    controllerSerial.write('S');
    digitalWrite(ceremonyLEDOut,LOW);
    digitalWrite(stopAllLEDOut,HIGH);
    digitalWrite(teleLEDOut,LOW);
    digitalWrite(autoLEDOut,LOW);
  }
  ////Temporary wiper code
  rightWipeHorizontal = analogRead(tempWiperControl);
  if ((rightWipeHorizontal > minStickActivation) && (rightWipeHorizontal < maxStickActivation)) {
    wipeVal = 0;
    wipeZero = true;
  }
  else {
    wipeVal = map(rightWipeHorizontal, 0, 1023, -127,127);
    wipeZero = false;
  }
   if (!wipeZero) {
    // Print right stick values if they are not zero
    Serial.print("W: ");
    Serial.println(wipeVal);
    controllerSerial.write('W');
    delay(5);
    controllerSerial.write(wipeVal);    
    delay(5); 
    zeroCountWipe = 0;
  }
   else if (zeroCountWipe < 4){
    controllerSerial.write('W');
    delay(5);
    wipeVal = 0;
    controllerSerial.write(wipeVal);
    delay(5);
    zeroCountWipe+=1;
  }
 controllerSerial.write("Q");
}
