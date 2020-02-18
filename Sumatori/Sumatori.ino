#include <SoftwareSerial.h>
#include <DualVNH5019MotorShield.h>
#include <QTRSensors.h>

// Motorshield Initializations
unsigned char INA1 = 40;
unsigned char INB1  = 42;
unsigned char PWM1 = 44;
unsigned char EN1DIAG1 = 38;
unsigned char CS1 = A6;
unsigned char INA2 = 41;
unsigned char INB2 = 43;
unsigned char PWM2 = 45;
unsigned char EN2DIAG2 = 39;
unsigned char CS2 = A7;
DualVNH5019MotorShield MS1;
DualVNH5019MotorShield MS2(INA1,INB1,PWM1,EN1DIAG1,CS1,INA2,INB2,PWM2,EN2DIAG2,CS2);

// Hall-Effect Sensor Initializations
unsigned char hallSensorIn = A8;

// Range Finder Initializations
unsigned char rangeFinderIn = A9;

//IR Reflectance Sensor Initializations
QTRSensors qtr;
const uint8_t reflectSensorCount = 8;
uint16_t reflectSensorValues[reflectSensorCount];

const int reflectSensorIn1 = 23;
const int reflectSensorIn2 = 25;
const int reflectSensorIn3 = 27;
const int reflectSensorIn4 = 29;
const int reflectSensorIn5 = 31;
const int reflectSensorIn6 = 33;
const int reflectSensorIn7 = 35;
const int reflectSensorIn8 = 37;
const int reflectSensorLED = 47;

// Stop Switch Initializations
const int stopSwitchRightSwiperIn = 18;
const int stopSwitchLeftSwiperIn = 19;
const int stopSwitchTopForkIn = 20;
const int stopSwitchBottomForkIn = 21;

// Value variables
int rightStickSpeed = 0;
bool rightStickDirection;
int8_t tempRightStickSpeed = 0;
int leftStickSpeed = 0;
int8_t tempLeftStickSpeed = 0;
bool leftStickDirection;
int forkLiftDirection = 0;
int wiperSpeed = 0;
int8_t tempWiperSpeed = 0;

void setup() {

  // Initialize Motor Shields
  MS1.init();
  MS2.init();
  
  // Hall-Effect Sensor Setup
  pinMode(hallSensorIn,INPUT);
  
  // Range Finder Sensor Setup
  pinMode(rangeFinderIn,INPUT);
  
  // Reflectance Sensors Setup
  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]){reflectSensorIn1, reflectSensorIn2, reflectSensorIn3, reflectSensorIn4,
                     reflectSensorIn5, reflectSensorIn6, reflectSensorIn7, reflectSensorIn8}, reflectSensorCount);
  qtr.setEmitterPin(reflectSensorLED);
  
  // Stopper Switch Setup
  pinMode(stopSwitchRightSwiperIn,INPUT);
  pinMode(stopSwitchLeftSwiperIn,INPUT);
  pinMode(stopSwitchTopForkIn,INPUT);
  pinMode(stopSwitchBottomForkIn,INPUT);
  
  // Serial Setup
  Serial3.begin(9600);
  Serial.begin(9600);
  }

void loop() {

// Read the serial port if there is something in it
while(Serial3.available() > 1){
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
    leftStickSpeed = map(tempLeftStickSpeed,-127,127,400,-400);
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
    MS2.setM1Speed(400);
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

  if (buttonPressed == 'W'){
     tempWiperSpeed = Serial3.read();
    wiperSpeed = map(tempWiperSpeed,-127,127,-400,400);
    Serial.print("W: ");
    Serial.println(wiperSpeed);
    MS2.setM2Speed(wiperSpeed);
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
