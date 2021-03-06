#include <SoftwareSerial.h>
#include <DualVNH5019MotorShield.h>
#include <QTRSensors.h>
#include <Encoder.h>

// DEBUGGING OPTIONS
#define debug // Uncomment to add debug mode (more verbosity)
#define sensor // Uncomment to print all sensor values
//#define serial // Uncomment to view serial debugging

// Serial Communications
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

// Motor encoder initializations
unsigned char leftWheelAout = 18; // Interrupt pins
unsigned char leftWheelBout = 19;
unsigned char rightWheelAout = 20;
unsigned char rightWheelBout = 21;

Encoder lmEnc(leftWheelAout, leftWheelBout); // Encoder intialization
Encoder rmEnc(rightWheelAout, rightWheelBout);

long rightEncoderCount = 0; // Counts
long leftEncoderCount = 0;

float leftWheelPosition = 0; // Positions and velocities
float leftWheelPositionOld = 0;
float leftWheelVelocity = 0;
float rightWheelPosition = 0;
float rightWheelPositionOld = 0;
float rightWheelVelocity = 0;

int startTimeRight = 0; // Encoder times 
int endTimeRight = 0;
int startTimeLeft = 0;
int endTimeLeft = 0;

float gearRatio = 102.08; // Gearing and encoder consts
int countsPerRev = 64;

float straightLineDistance = 32.0; // cm PM10 PARAMETERS  
float turnRadius = 8.0; // cm

float wheelDiameter = 10; // cm
float wheelSpacing = 21; // cm
float straightLineRadians = straightLineDistance * 2 / (wheelDiameter / 2.54); // 2*pi*dist/(pi*D)
float wheelTurnProportion = turnRadius / (turnRadius + wheelSpacing);

//TEMP VARIABLE FOR COUNTING FOR PRINT STUFF
int count = 0;
int leftStraightLineSpeed = 250;
int rightStraightLineSpeed = 250;

// Hall-Effect Sensor Initializations
unsigned char hallSensorIn = A9;
uint16_t hallSensorVal = 512;
const uint16_t hallEffectActivation = 295;
const uint16_t hallSensorMax = 1023-hallEffectActivation;
const uint16_t hallSensorMin = hallEffectActivation;
const double hallSensorAlpha = 0.6;

// Range Finder Initializations
unsigned char rangeFinderIn = A8;
uint8_t optimalDistance = 15;
uint8_t optimalDistanceRange = 5;
uint16_t minRangeVal = int(13*pow(optimalDistance+optimalDistanceRange, -.92)/5 * 1023);
uint16_t maxRangeVal = int(13*pow(optimalDistance-optimalDistanceRange, -.92)/5 * 1023);
const double rangeFinderAlpha = 0.6;
uint16_t distanceValue = 0;
uint8_t rangeThreshold = 35;

//IR Reflectance Sensor Initializations
QTRSensors qtr;

const int8_t reflectSensorCount = 8;
int16_t reflectSensorValues[reflectSensorCount];
uint16_t biasArray[reflectSensorCount];
const float lineFollowingSpeedMultiplier = .55;

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
const int stopSwitchRightSwiperIn = 22;
const int stopSwitchLeftSwiperIn = 24;
const int stopSwitchTopForkIn = 26;
const int stopSwitchBottomForkIn = 28;

// Value variables
int rightStickSpeed = 0;
bool rightStickDirection;
int8_t tempRightStickSpeed = 0;
int leftStickSpeed = 0;
int8_t tempLeftStickSpeed = 0;
bool leftStickDirection;
char forkLiftVal = 'Z';
int wiperSpeed = 0;
int8_t tempWiperSpeed = 0;

// Entrance ceremony
uint8_t entranceStep = 0;

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

  // Initialize Motor Shields (encoders have no inits in setup)
  MS1.init();
  MS2.init();

  // Hall-Effect Sensor Setup
  pinMode(hallSensorIn,INPUT);
  
  // Range Finder Sensor Setup
  pinMode(rangeFinderIn,INPUT);

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

  // Read encoder values
  leftEncoderCount = lmEnc.read();
  rightEncoderCount = rmEnc.read();
  
  // Read controller inputs
  ParseSerialComms();
  
  if (teleoperatedFlag){
    Teleoperation(); 
  }
  else if (autonomousFlag) {
    // LineFollowing();
    StraightLine();
  }
  else if (entranceFlag){
    if (entranceStep == 0){
      if (HallEffect()){
        #ifdef debug
          Serial.println("Moving to step 1!");
        #endif
        entranceStep++;
      }
    }
    else if (entranceStep == 1){
      if (!WallFollowing()){
        #ifdef debug
          Serial.println("Moving to step 2!");
        #endif
        entranceStep++;
      }
    }
    else if (entranceStep == 2){
      StopCommand();
      entranceStep = 0;
    }
  }
  else if (stopFlag) {
    // Do Nothing in here
  }


}
