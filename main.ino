#include "CytronMotorDriver.h"
#include <Wire.h>
#include "Adafruit_TCS34725.h"

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);
// Configure the motor driver.
CytronMD motor1(PWM_DIR, 2, 3);  // PWM 1 = Pin 3, DIR 1 = Pin 4.
CytronMD motor2(PWM_DIR, 5, 4); // PWM 2 = Pin 9, DIR 2 = Pin 10.
//Ultrasonic sensor ********************************************************************************************************************************************************************************
#define echoFor 14
#define echoRgh 15
#define echoLft 16
#define trigPin 17

long duration1 = 0;
long duration2 = 0;
long duration3 = 0;
int distance1 = 0;
int distance2 = 0;
int distance3 = 0;
//*********************************************************************************************************************************************************************************************
bool alert = false;
const int alert_distance = 20;
void setup() {
  //Ultrasonic
  pinMode(trigPin, OUTPUT);
  pinMode(echoFor, INPUT);
  pinMode(echoRgh, INPUT);
  pinMode(echoLft, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  check_distance_sensors();
  motors("left");

}
int read_color_sensor(){
  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);
  
}
int motors(char state){
  if(state = "forward"){
    motor1.setSpeed(128);   // Motor 1 runs forward at 50% speed.
    motor2.setSpeed(-128);
  }  
  if(state = "left"){
    motor1.setSpeed(200);   // Motor 1 runs forward at 50% speed.
    motor2.setSpeed(-128);
  }  
  if(state = "right"){
    motor1.setSpeed(128);   // Motor 1 runs forward at 50% speed.
    motor2.setSpeed(-200);
  }  
  if(state = "brake"){
    motor1.setSpeed(0);   // Motor 1 runs forward at 50% speed.
    motor2.setSpeed(0);
  }  
}
int check_distance_sensors() {
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
  distance1 = duration1 * 0.034 / 2;
  distance2 = duration2 * 0.034 / 2;
  distance3 = duration3 * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  //Serial.print("Distance: ");
  //Serial.println(distance1);
  
  if (int(duration1) > alert_distance || int(duration2) > alert_distance || int(duration3) > alert_distance) {
    alert == true;
    Serial.print(alert);
    Serial.println("Alert Status");
  }
}
