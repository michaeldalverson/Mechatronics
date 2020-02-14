#include <SoftwareSerial.h>
//Serial Communication

SoftwareSerial controllerSerial(2,3);

//Analog Pins
//Wheel control sticks
const int leftStickVertIn = A0;
const int rightStickVertIn = A1;

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
const int forkLiftRightUpIn = 12;
const int forkLiftLeftDownIn = 13;

//Value Variables
bool buttonPressedT = false;
bool buttonPressedE = false;
//Buttons
bool entranceCeremony;
bool autonomousWrestling;
bool teleWrestling;
bool STOP;

//Left Wheel
uint16_t leftStickVal;
int8_t leftStickSpeed;
bool leftStickDirection;

//Right Wheel
uint16_t rightStickVal;
int8_t rightStickSpeed;
bool rightStickDirection;

//Forklift
bool forkUp;
bool forkDown;

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


  //Serial
 
  controllerSerial.begin(9600);
  Serial.begin(9600);
}

void loop() {

  //Read Left Stick
      leftStickVal = analogRead(leftStickVertIn);
      if ((leftStickVal > 490) && (leftStickVal < 530)) {
        leftStickSpeed = 0;
      }
      else {
        leftStickSpeed = map(leftStickVal, 0, 1023, -400,400);
      
      }

  //Read Right Stick
      rightStickVal = analogRead(rightStickVertIn);
      if ((rightStickVal > 490) && (rightStickVal < 530)) {
        rightStickSpeed = 0;
 
      }
      else {
        rightStickSpeed = map(rightStickVal, 0, 1023, 400,-400);
   
      }

      controllerSerial.write('L');
      controllerSerial.write(leftStickSpeed);
      Serial.println(leftStickSpeed);
      //controllerSerial.write(leftStickDirection);
    
      delay(50);

      controllerSerial.write('R');
      controllerSerial.write(rightStickSpeed);
      //controllerSerial.write(rightStickDirection);

      delay(50);
      

   //Forklift
      forkUp = digitalRead(forkLiftRightUpIn);
      forkDown = digitalRead(forkLiftLeftDownIn);
      if ((forkUp) && (!forkDown)) {
        controllerSerial.write('U');
        delay(25);
      }
      else if ((forkDown) && (!forkUp)) {
        controllerSerial.write('D');
        delay(25);
      }
      else {
        //Do nothing
      }

  entranceCeremony = digitalRead(ceremonyIn);
  teleWrestling = digitalRead(teleIn);
  autonomousWrestling = digitalRead(autoIn);
  STOP = digitalRead(stopAllIn);

  if (entranceCeremony){
    controllerSerial.write('E');
    delay(25);
    digitalWrite(ceremonyLEDOut,HIGH);
    digitalWrite(stopAllLEDOut,LOW);
    digitalWrite(teleLEDOut,LOW);
    digitalWrite(autoLEDOut,LOW);
  }
  if (teleWrestling){
    controllerSerial.write('T');
     digitalWrite(ceremonyLEDOut,LOW);
    digitalWrite(stopAllLEDOut,LOW);
    digitalWrite(teleLEDOut,HIGH);
    digitalWrite(autoLEDOut,LOW);
  }
   if (autonomousWrestling){
    controllerSerial.write('A');
     digitalWrite(ceremonyLEDOut,LOW);
    digitalWrite(stopAllLEDOut,LOW);
    digitalWrite(teleLEDOut,LOW);
    digitalWrite(autoLEDOut,HIGH);
  }
   if (STOP){
    controllerSerial.write('S');
    digitalWrite(ceremonyLEDOut,LOW);
    digitalWrite(stopAllLEDOut,HIGH);
    digitalWrite(teleLEDOut,LOW);
    digitalWrite(autoLEDOut,LOW);
  }
  /**
  entranceCeremony = digitalRead(ceremonyIn);
  teleWrestling = digitalRead(teleIn);
  autonomousWrestling = digitalRead(autoIn);
  STOP = digitalRead(stopAllIn);
  if (!STOP) {
    if (!buttonPressedE) {
      if (entranceCeremony) {
        buttonPressedE = true;
        controllerSerial.write('E') ;
        delay(25);
      }
    }
    if ((buttonPressedE) && (!buttonPressedT)) {
      if (autonomousWrestling) {
        controllerSerial.write('A');
        delay(25);
      }
    }
    else if (!buttonPressedT) {
      if (teleWrestling) {
        buttonPressedT = true;
        buttonPressedE = true;
        controllerSerial.write('T');
        delay(25);
      }
    } else {
      /**
         flag
         speed
         direction
      
      //Read Left Stick
      leftStickVal = analogRead(leftStickVertIn);
      if ((leftStickVal > 510) && (leftStickVal < 513)) {
        leftStickSpeed = 0;
        leftStickDirection = false;
      }
      else if (leftStickVal > 511) {
        leftStickSpeed = map(leftStickVal, 512, 1023, 0, 255);
        leftStickDirection = true;
      }
      else {
        leftStickSpeed = map(leftStickVal, 0, 511, 0, 255);
        leftStickDirection = false;
      }

      //Read Right Stick
      rightStickVal = analogRead(rightStickVertIn);
      if ((rightStickVal > 510) && (rightStickVal < 513)) {
        rightStickSpeed = 0;
        rightStickDirection = false;
      }
      else if (rightStickVal > 511) {
        rightStickSpeed = map(rightStickVal, 512, 1023, 0, 255);
        rightStickDirection = true;
      }
      else {
        rightStickSpeed = map(rightStickVal, 0, 511, 0, 255);
        rightStickDirection = false;
      }

      controllerSerial.write('L');
      controllerSerial.write(leftStickSpeed);
      controllerSerial.write(leftStickDirection);
      delay(25);

      controllerSerial.write('R');
      controllerSerial.write(rightStickSpeed);
      controllerSerial.write(rightStickDirection);
      delay(25);

      //Forklift
      forkUp = digitalRead(forkLiftRightUpIn);
      forkDown = digitalRead(forkLiftLeftDownIn);
      if ((forkUp) && (!forkDown)) {
        controllerSerial.write('U');
        delay(25);
      }
      else if ((forkDown) && (!forkUp)) {
        controllerSerial.write('D');
        delay(25);
      }
      else {
        //Do nothing
      }


    }
  }
  else {
    bool stopped = true;
    while (stopped) {

    }
  }
  **/
  
}
