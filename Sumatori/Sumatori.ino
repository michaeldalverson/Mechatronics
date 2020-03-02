#include <SoftwareSerial.h>
#include <DualVNH5019MotorShield.h>
#include <QTRSensors.h>

// DEBUGGING OPTIONS
#define debug // Uncomment to add debug mode (more verbosity)
//#define sensor // Uncomment to print all sensor values
#define serial // Uncomment to view serial debugging

// Serial Communications
const uint8_t serialBufferSize = 16;
char serialBufferArray[serialBufferSize] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
char serialVal;

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
uint16_t hallSensorVal = 512;

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
float lineFollowingSpeedMultiplier = .55;

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

ParseSerialComms();

if (autonomousFlag) {
  LineFollowing();
}
else if (entranceFlag){
  HallEffect();
  //WallFollowing();
}
else if (stopFlag) {
  StopCommand();
}


}
