#include <SoftwareSerial.h>
#include <DualVNH5019MotorShield.h>
#include <QTRSensors.h>

// DEBUGGING OPTIONS
#define debug // Uncomment to add debug mode (more verbosity)
#define sensor // Uncomment to print all sensor values

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

// Motor hall-effect sensor intializations
unsigned char forkliftAout = A3;
unsigned char forkliftBout = A2;
unsigned char wiperAout = A5;
unsigned char wiperBout = A4;
unsigned char rightWheelAout = A11;
unsigned char rightWheelBout = A10;
unsigned char leftWheelAout = A13;
unsigned char leftWheelBout = A12;

// Hall-Effect Sensor Initializations
unsigned char hallSensorIn = A9;

// Range Finder Initializations
unsigned char rangeFinderIn = A8;
uint8_t optimalDistance = 15;
uint8_t optimalDistanceRange = 5;
uint16_t minRangeVal = int(13*pow(optimalDistance+optimalDistanceRange, -.92)/5 * 1023);
uint16_t maxRangeVal = int(13*pow(optimalDistance-optimalDistanceRange, -.92)/5 * 1023);

//Rangefinder filtering
double rangeFinderAlpha = 0.2;
uint16_t distanceValue = 0;

//IR Reflectance Sensor Initializations
QTRSensors qtr;

const int8_t reflectSensorCount = 8;
int16_t reflectSensorValues[reflectSensorCount];
uint16_t biasArray[reflectSensorCount];

const int reflectSensorIn1 = 23;
const int reflectSensorIn2 = 25;
const int reflectSensorIn3 = 27;
const int reflectSensorIn4 = 29;
const int reflectSensorIn5 = 31;
const int reflectSensorIn6 = 33;
const int reflectSensorIn7 = 35;
const int reflectSensorIn8 = 37;
const int reflectSensorLED = 22;

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

// Controller Mode
bool teleoperatedFlag = false;
bool stopFlag = false;
bool entranceFlag = false;
bool autonomousFlag = false;

//Autonomous turning bounds
int bound = 0.25;

void setup() {
  
  // Serial Setup
  Serial3.begin(9600);
  Serial.begin(9600);

  // Initialize Motor Shields
  MS1.init();
  MS2.init();

  // Motor hall-effect sensor setup
  pinMode(hallSensorIn,INPUT);
  
  // Hall-Effect Sensor Setup
  pinMode(hallSensorIn,INPUT);
  
  // Range Finder Sensor Setup
  pinMode(forkliftAout,INPUT);
  pinMode(forkliftBout,INPUT);
  pinMode(wiperAout,INPUT);
  pinMode(wiperAout,INPUT);
  pinMode(rightWheelAout,INPUT);
  pinMode(rightWheelAout,INPUT);
  pinMode(leftWheelAout,INPUT);
  pinMode(leftWheelAout,INPUT);

  #ifdef debug
    Serial.print("Range Finder Optimal Vals: ");
    Serial.print(minRangeVal);
    Serial.print(" - ");
    Serial.println(maxRangeVal);
  #endif
  
  // Reflectance Sensors Setup
  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]){reflectSensorIn1, reflectSensorIn2, reflectSensorIn3, reflectSensorIn4,
                     reflectSensorIn5, reflectSensorIn6, reflectSensorIn7, reflectSensorIn8}, reflectSensorCount);
  qtr.setEmitterPin(reflectSensorLED);

  // Setup bias for qtr
  #ifdef debug
    Serial.println("Biasing QTR");
  #endif
  LineFollowingBias();
  
  // Stopper Switch Setup
  pinMode(stopSwitchRightSwiperIn,INPUT);
  pinMode(stopSwitchLeftSwiperIn,INPUT);
  pinMode(stopSwitchTopForkIn,INPUT);
  pinMode(stopSwitchBottomForkIn,INPUT);
 
}

void loop() { 

// Read the serial port if there is something in it
while(Serial3.available() > 1){
  // Read char
  char buttonPressed = Serial3.read();

  // MIDDLE BUTTONS
  if (buttonPressed == 'E'){
    #ifdef debug
      Serial.println("Entrance Received");
    #endif
    teleoperatedFlag = false;
    stopFlag = false;
    entranceFlag = true;
    autonomousFlag = false;

    // Turn off all brakes
    MS1.setM1Brake(0);
    MS1.setM2Brake(0);
    MS2.setM1Brake(0);
    MS2.setM2Brake(0);
    
    delay(2);
  }
  if (buttonPressed == 'S'){
    #ifdef debug
      Serial.println("Stop Received");
    #endif
    teleoperatedFlag = false;
    stopFlag = true;
    entranceFlag = false;
    autonomousFlag = false;

    // Turn motors off 
    MS1.setM1Speed(0);
    MS1.setM2Speed(0);
    MS2.setM1Speed(0);
    MS2.setM2Speed(0);

    // Turn on all brakes
    MS1.setM1Brake(400);
    MS1.setM2Brake(400);
    MS2.setM1Brake(400);
    MS2.setM2Brake(400);
    
    delay(2);
  }
  if (buttonPressed == 'A'){
    #ifdef debug
      Serial.println("Autonomous Received");
    #endif
    teleoperatedFlag = false;
    stopFlag = false;
    entranceFlag = false;
    autonomousFlag = true;

    // Turn off all brakes
    MS1.setM1Brake(0);
    MS1.setM2Brake(0);
    MS2.setM1Brake(0);
    MS2.setM2Brake(0);
    
    delay(2);
  }
  if (buttonPressed == 'T'){
    #ifdef debug
      Serial.println("Teleoperated Received");
    #endif
    teleoperatedFlag = true;
    stopFlag = false;
    entranceFlag = false;
    autonomousFlag = false;

    // Turn motors off when teleop is pressed
    MS1.setM1Speed(0);
    MS1.setM2Speed(0);
    MS2.setM1Speed(0);
    MS2.setM2Speed(0);

    // Turn off all brakes
    MS1.setM1Brake(0);
    MS1.setM2Brake(0);
    MS2.setM1Brake(0);
    MS2.setM2Brake(0);
    
    delay(2);
  }

  // Control Values
  if (teleoperatedFlag) {
    if (buttonPressed == 'L'){
      tempLeftStickSpeed = Serial3.read();
      leftStickSpeed = map(tempLeftStickSpeed,-127,127,400,-400);
      #ifdef debug
        Serial.print("L: ");
        Serial.println(leftStickSpeed);
      #endif
      MS1.setM1Speed(leftStickSpeed);
    }
  
    if (buttonPressed == 'R'){
      tempRightStickSpeed = Serial3.read();
      rightStickSpeed = map(tempRightStickSpeed,-127,127,-400,400);
      #ifdef debug
        Serial.print("R: ");
        Serial.println(rightStickSpeed);
      #endif
      MS1.setM2Speed(rightStickSpeed);
    }
    
    if(buttonPressed == 'U'){
      #ifdef debug
        Serial.println("Fork Up");
      #endif
      MS2.setM1Brake(0);
      MS2.setM1Speed(400);
    }
  
    if (buttonPressed == 'D'){
      #ifdef debug
        Serial.println("Fork Down");
      #endif
      MS2.setM1Brake(0);
      MS2.setM1Speed(-400);
    }
  
    if (buttonPressed == 'F'){
      #ifdef debug
        Serial.println("Fork Locked");
      #endif
      MS2.setM1Speed(0);
      MS2.setM1Brake(400);
    }
  
    if (buttonPressed == 'W'){
      tempWiperSpeed = Serial3.read();
      wiperSpeed = map(tempWiperSpeed,-127,127,-400,400);
      #ifdef debug
        Serial.print("W: ");
        Serial.println(wiperSpeed);
      #endif
      MS2.setM2Speed(wiperSpeed);
    }
  }
  else {
    Serial3.read();
  }
  
}

if (autonomousFlag) {
  autonomousWrestlingPM7();
}
else if (entranceFlag){
  WallFollowing();
}
else if (stopFlag) {
  // Unimplemented
}


}
