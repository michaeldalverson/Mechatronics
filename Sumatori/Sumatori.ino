#include <SoftwareSerial.h>
#include "DualVNH5019MotorShield.h"

DualVNH5019MotorShield MS1;
DualVNH5019MotorShield MS2;

//Analog Pins
//Sensors
const int hallSensorIn = A0;
const int rangeFinderIn = A1;

//Digital Pins
//IR Reflectance Sensors
const int reflectSensorIn1 = 2;
const int reflectSensorIn2 = 3;
const int reflectSensorIn3 = 4;
const int reflectSensorIn4 = 5;
const int reflectSensorIn5 = 6;
const int reflectSensorIn6 = 7;
const int reflectSensorIn7 = 8;

//Manipulator and Wheel Control
const int wiperMotorOut = 10;
const int forkliftMotorOut =11;
const int rightMotorDriveOut = 12;
const int leftMotorDriveOut = 13;

//Switches to indicate forklift and wiper extremes
const int stopSwitchRightSwiperIn = 18;
const int stopSwitchLeftSwiperIn = 19;
const int stopSwitchTopForkIn = 20;
const int stopSwitchBottomForkIn = 21;

//Value variables
int rightStickSpeed = 0;
bool rightStickDirection;
int8_t tempRightStickSpeed = 0;
int leftStickSpeed = 0;
int8_t tempLeftStickSpeed = 0;
bool leftStickDirection;
int forkLiftDirection = 0;

void setup() {

// Initialize Motor Shields
MS1.init();
MS2.init();

//Sensors
pinMode(hallSensorIn,INPUT);
pinMode(rangeFinderIn,INPUT);

//Reflectance Sensors
pinMode(reflectSensorIn1,INPUT);
pinMode(reflectSensorIn2,INPUT);
pinMode(reflectSensorIn3,INPUT);
pinMode(reflectSensorIn4,INPUT); 
pinMode(reflectSensorIn5,INPUT);
pinMode(reflectSensorIn6,INPUT);
pinMode(reflectSensorIn7,INPUT);

pinMode(wiperMotorOut,OUTPUT);
pinMode(forkliftMotorOut,OUTPUT);
pinMode(rightMotorDriveOut,OUTPUT);
pinMode(leftMotorDriveOut,OUTPUT);

pinMode(stopSwitchRightSwiperIn,INPUT);
pinMode(stopSwitchLeftSwiperIn,INPUT);
pinMode(stopSwitchTopForkIn,INPUT);
pinMode(stopSwitchBottomForkIn,INPUT);

//Serial
Serial3.begin(9600);
Serial.begin(9600);
}

void loop() {

while(Serial3.available() > 2){
  char buttonPressed = Serial3.read();
  if (buttonPressed == 'E'){
    Serial.println("Entrance Ceremony Commence");
    delay(2);
  }
  if (buttonPressed == 'S'){
    Serial.println("STOP");
    delay(2);
  }
  if (buttonPressed == 'A'){
    Serial.println("Autonomous Wrestling Mode");
    delay(2);
  }
  if (buttonPressed == 'T'){
    Serial.println("Teleoperated Wrestling Mode");
    delay(2);
  }
  
  if (buttonPressed == 'L'){
    tempLeftStickSpeed = Serial3.read();
    leftStickSpeed = map(tempLeftStickSpeed,-127,127,-400,400);
    Serial.print("L: ");
    Serial.println(leftStickSpeed);
    MS1.setM1Speed(leftStickSpeed);
  }
  
  if (buttonPressed == 'R'){
    tempRightStickSpeed = Serial3.read();
    rightStickSpeed = map(tempRightStickSpeed,-127,127,-400,400);
    Serial.print("R: ");
    Serial.println(rightStickSpeed);
    MS1.setM2Speed(rightStickSpeed);
  }
 
  if(buttonPressed == 'U'){
    Serial.println("Fork Up");
    MS2.setM1Brake(0);
    MS1.setM1Speed(400);
  }
  
  if (buttonPressed == 'D'){
    Serial.println("Fork Down");
    MS2.setM1Brake(0);
    MS2.setM1Speed(-400);
  }

  if (buttonPressed == 'F'){
    Serial.println("Fork Locked");
    MS2.setM1Speed(0);
    MS2.setM1Brake(400);
  }
}
/*
  while(Serial1.available() > 2){

  
    if(Serial1.read() == 'E'){
      void entranceCeremony();
    }
     if(Serial1.read() == 'A'){
      void autonomousWrestling();
  }
    if(Serial1.read() == 'T'){
     void teleOperation();    
    }
   if (Serial1.read() == 'L'){
    leftStickSpeed = Serial1.read();
    leftStickDirection = Serial1.read();
    
   }
   if(Serial1.read() =='R'){
    rightStickSpeed = Serial.read();
    rightStickDirection = Serial1.read();
   }
   if(Serial1.read() == 'U'){
    forkLiftDirection = 1;
   }
   if(Serial1.read() == 'D'){
    forkLiftDirection = -1;
   }

}
*/
}

void entranceCeremony(){
  return;
}
void autonomousWrestling(){
  return;
}
void teleOperation(){
  return;
}
