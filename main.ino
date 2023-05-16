#include "Adafruit_TCS34725.h"
#include "CytronMotorDriver.h"
#include <QTRSensors.h>
#include <Wire.h>

#include"pinDefines.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define KP .2
#define KD 5
#define M1_DEFAULT_SPEED 50
#define M2_DEFAULT_SPEED 50
#define M1_MAX_SPEED 70
#define M2_MAX_SPEED 70
#define MIDDLE_SENSOR 4
#define NUM_SENSORS  8     // брой сензори
#define TIMEOUT       2500  // таимоут
#define EMITTER_PIN   2     //емитер
#define DEBUG 0 // задай на 1 за дебъг режим

QTRSensorsRC qtrrc((unsigned char[]) {6, 7, 8, 9, 10, 11, 12, 13}, NUM_SENSORS, TIMEOUT, EMITTER_PIN);

unsigned int sensorValues[NUM_SENSORS];

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);
CytronMD motor1(PWM_DIR, 2, 3);  // PWM 1 = Pin 3, DIR 1 = Pin 4.
CytronMD motor2(PWM_DIR, 5, 4); // PWM 2 = Pin 9, DIR 2 = Pin 10.
.
int distance1 = 0;
int distance2 = 0;
int distance3 = 0;

bool alert = false;
const int ALERT_DISTANCE = 20; //Добра практика е да се използва "ALL_CAPS" стилът за именуване на константи както съм показал

class motors{
  public:
  int speed = 200;
    void forward(){
      motor1.setSpeed(speed);   
      motor2.setSpeed(-speed);
    }
    void right(){
      motor1.setSpeed(128);   
      motor2.setSpeed(-speed);
    }
    void left(){
      motor1.setSpeed(speed);   
      motor2.setSpeed(-128);
    }
    void brake(){
      motor1.setSpeed(0);
      motor2.setSpeed(0);
    }
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() 
{
  //declarePins();
  manual_calibration(); 

  Serial.begin(9600);
}

void loop() {
  motors motors;
  check_distance_sensors();
  motors.left();

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void manual_calibration() {

  int i;
  for (i = 0; i < 250; i++)  // the calibration will take a few seconds
  {
    qtrrc.calibrate(QTR_EMITTERS_ON);
    delay(20);
  }

  if (DEBUG) { // if true, generate sensor dats via serial output
    Serial.begin(9600);
    for (int i = 0; i < NUM_SENSORS; i++)
    {
      Serial.print(qtrrc.calibratedMinimumOn[i]);
      Serial.print(' ');
    }
    Serial.println();

    for (int i = 0; i < NUM_SENSORS; i++)
    {
      Serial.print(qtrrc.calibratedMaximumOn[i]);
      Serial.print(' ');
    }
    Serial.println();
    Serial.println();
  }
}

void read_color_sensor(){  //незнам как работи тази библиотека, нищо няма да барам тук засега.
  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);
  
}

void check_distance_sensors() {
  long duration1 = 0;
  long duration2 = 0;
  long duration3 = 0;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration1 = pulseIn(echoLft, HIGH);
  duration2 = pulseIn(echoRgh, HIGH);
  duration3 = pulseIn(echoFor, HIGH);

  // Calculating the distance
  distance1 = convertDurationToCM(duration1); // Въпреки че неспестява код, това ще направи кода по-лесно четим за страничен наблювател.
  distance2 = convertDurationToCM(duration2);
  distance3 = convertDurationToCM(duration3);

  // Prints the distance on the Serial Monitor
  //Serial.print("Distance: ");
  //Serial.println(distance1);
  
  if (int(duration1) > ALERT_DISTANCE || int(duration2) > ALERT_DISTANCE || int(duration3) > ALERT_DISTANCE) {
    alert == true;
    //Serial.print(alert);
    Serial.println("Alert Status");// Попринцип всички Serial-команди се водят "Бавни" операции. Ако имаш възможност ги избягвай. (Това би важало и за моя код на ред 57).
                                  //Ако сметнеш че неискаш да стоят, все още не е нужно да бързаш да ги махаш, тъй като са полезни за дебъгване
  }
}

int convertDurationToCM(long duration){
  return duration * 0.034 / 2; //Формулата ти е взех от горните редове, не съм е проверявал дали е коректна.
}
