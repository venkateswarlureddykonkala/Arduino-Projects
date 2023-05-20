#include<NewPing.h>
#include<Servo.h>
#include <AFMotor.h>
#define TRIGGER_PIN A1
#define ECHO_PIN A2 
#define MAX_DISTANCE 250
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
Servo servomotor;
AF_DCMotor m1(1);
AF_DCMotor m2(2);
AF_DCMotor m3(3);
AF_DCMotor m4(4);
int distance;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  servomotor.attach(10);
  servomotor.write(90);
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  int distanceright;
  int distanceleft;
  int distance = readPing();
  Serial.println(distance);
  delay(100);
  if(Serial.available()>0)
  {
    moveforward();
  }
  if(distance <= 20)
  {
    movestop();
    delay(300);
    movebackward();
    delay(400);
    movestop();
    delay(200);
    distanceright = lookright();
    delay(200);
    distanceleft = lookleft();
    delay(200);

    if(distanceright >= distanceleft)
    {
      turnright();
      delay(600);
      moveforward();
    }
    if(distanceright <= distanceleft)
    {
      turnleft();
      delay(600);
      moveforward();
    }
  }
  else
  {
    moveforward();
  }
  

}

int lookright()
{
  servomotor.write(0);
  delay(500);
  int distancee = readPing();
  delay(500);
  servomotor.write(90);
  delay(500);
  return distancee;
  Serial.println(distance);
  
  
}

int lookleft()
{
  servomotor.write(180);
  delay(500);
  int distances = readPing();
  delay(500);
  servomotor.write(90);
  delay(500);
  return distances;
   Serial.println(distance);
}
int readPing()
{
  delay(100);
  int cm = sonar.ping_cm();
   if(cm==0)
  {
    cm=250;
  }
  return cm;
}
void moveforward()
{
  m1.setSpeed(225);
  m1.run(FORWARD);
  m2.setSpeed(225);
  m2.run(FORWARD);
  m3.setSpeed(225);
  m3.run(FORWARD);
  m4.setSpeed(225);
  m4.run(FORWARD);
  
}
void movebackward()
{
  m1.setSpeed(225);
  m1.run(BACKWARD);
  m2.setSpeed(225);
  m2.run(BACKWARD);
  m3.setSpeed(225);
  m3.run(BACKWARD);
  m4.setSpeed(225);
  m4.run(BACKWARD);
  
}

void turnleft()
{
  m1.setSpeed(225);
  m1.run(BACKWARD);
  m2.setSpeed(225);
  m2.run(BACKWARD);
  m3.setSpeed(225);
  m3.run(FORWARD);
  m4.setSpeed(225);
  m4.run(FORWARD);
  
}

void turnright()
{
  m1.setSpeed(225);
  m1.run(FORWARD);
  m2.setSpeed(225);
  m2.run(FORWARD);
  m3.setSpeed(225);
  m3.run(BACKWARD);
  m4.setSpeed(225);
  m4.run(BACKWARD);
  
}

void movestop()
{
  m1.setSpeed(0);
  m2.setSpeed(0);
  m3.setSpeed(0);
  m4.setSpeed(0);
}
