#include <SoftwareSerial.h>

//#define inputs // Uncomment to print all input values from controller components
#define outputs // Uncomment to print all sent serial values

//Serial Communication
SoftwareSerial controllerSerial(2,3);
String serialWriteString = "";
uint8_t commands = 0;

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
int zeroCountLeft = 4;

//Right Wheel
uint16_t rightStickVal;
int8_t rightStickSpeed;
bool rightStickZero = true;
int zeroCountRight = 4;

//Wiper 
uint16_t rightWipeHorizontal;
int8_t wipeVal;
bool wipeZero = true;
int zeroCountWipe = 4;

//Forklift
bool forkUp;
bool forkDown;
char forkState;
bool forkZero = true;

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
  
  delay(75);
  
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
    leftStickSpeed = map(leftStickVal, 0, 1023, -127, 127);
    leftStickZero = false;
  }

  //Right Stick 
  if ((rightStickVal > minStickActivation) && (rightStickVal < maxStickActivation)) {
    rightStickSpeed = 0;
    rightStickZero = true;
  }
  else {
    rightStickSpeed = map(rightStickVal, 0, 1023, 127, -127);
    rightStickZero = false;
  }
  
  //Temporary wiper code
  if ((rightWipeHorizontal > minStickActivation) && (rightWipeHorizontal < maxStickActivation)) {
    wipeVal = 0;
    wipeZero = true;
  }
  else {
    wipeVal = map(rightWipeHorizontal, 0, 1023, -127, 127);
    wipeZero = false;
  }

  // Forklift
  if ((forkUp) && (!forkDown)) {
    forkZero = false;
    forkState = 'U';
  }
  else if ((forkDown) && (!forkUp)) {
    forkState = 'D';
    forkZero = false;
  }
  else if (!forkZero){
    forkState = 'F';
    forkZero = true;
  }
  else {
    forkState = 'Z';
  }

  // ***************************** BUILD PACKETS ******************************
  // Begin with start of packet
  serialWriteString += char(2);
 
  // Write Left Stick if they are not zero
  if (!leftStickZero) {
    zeroCountLeft = 0;
    serialWriteString += 'L';
    serialWriteString += char(leftStickSpeed);
    commands++;
  }
  else if (zeroCountLeft <4){
    leftStickSpeed = 0;
    serialWriteString += 'L';
    serialWriteString += char(1);
    commands++;
    zeroCountLeft++;
  }

  // Write Right Stick if they are not zero
  if (!rightStickZero) {
    serialWriteString += 'R';
    serialWriteString += char(rightStickSpeed);
    commands++;
    zeroCountRight = 0;
  }
  else if (zeroCountRight < 4){
    rightStickSpeed = 0;
    serialWriteString += 'R';
    serialWriteString += char(1);
    commands++;
    zeroCountRight++;
  }

  // Temp Wiper write
  if (!wipeZero) {
    serialWriteString += 'W';
    serialWriteString += char(wipeVal);
    commands++;
    zeroCountWipe = 0;
  }
  else if (zeroCountWipe < 4){
    wipeVal = 0;
    serialWriteString += 'W';
    serialWriteString += char(1);
    commands++;
    zeroCountWipe++;
  }

  // WRITE MID VALUE
  serialWriteString += char(3);

  // Write Fork State
  if (forkState != 'Z'){
    serialWriteString += forkState;
    commands++;
  }

  // MAIN BUTTONS & LED CONTROL
  if (entranceCeremony){
    serialWriteString += 'E';
    commands++;
    digitalWrite(ceremonyLEDOut,HIGH);
    digitalWrite(stopAllLEDOut,LOW);
    digitalWrite(teleLEDOut,LOW);
    digitalWrite(autoLEDOut,LOW);
  }
  if (teleWrestling){
    serialWriteString += 'T';
    commands++;
    digitalWrite(ceremonyLEDOut,LOW);
    digitalWrite(stopAllLEDOut,LOW);
    digitalWrite(teleLEDOut,HIGH);
    digitalWrite(autoLEDOut,LOW);
  }
   if (autonomousWrestling){
    serialWriteString += 'A';
    commands++;
    digitalWrite(ceremonyLEDOut,LOW);
    digitalWrite(stopAllLEDOut,LOW);
    digitalWrite(teleLEDOut,LOW);
    digitalWrite(autoLEDOut,HIGH);
  }
   if (STOP){
    serialWriteString += 'S';
    commands++;
    digitalWrite(ceremonyLEDOut,LOW);
    digitalWrite(stopAllLEDOut,HIGH);
    digitalWrite(teleLEDOut,LOW);
    digitalWrite(autoLEDOut,LOW);
  }

  // ***************************** SERIAL WRITES ******************************
  if (commands > 0){
    // *** WARNING *** SENDING 0 USING CHARS DOES NOT WORK BECAUSE IT TERMINATES THE PACKET STRING
    serialWriteString += char(4);

    // Convert string into char array using this magic
    char* buf = (char*) malloc(sizeof(char)*serialWriteString.length()+1);
    serialWriteString.toCharArray(buf, serialWriteString.length()+1);
  
    // Send char array packet to mega
    controllerSerial.write(buf);
    
    #ifdef outputs
      Serial.print("L: ");
      Serial.print(serialWriteString.length());
      Serial.print(" Packet: ");
      Serial.print(buf);
      Serial.println();
    #endif

    // Deallocated the buffer char array and reset counter
    free(buf);
    commands = 0;
  }
  // Reset packet string
  serialWriteString = "";

}
