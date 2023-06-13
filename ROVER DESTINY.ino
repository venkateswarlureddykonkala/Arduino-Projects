#include <Wire.h>
#include <AFMotor.h>
#include <QMC5883LCompass.h>
#include <SoftwareSerial.h>
#include <math.h>
#include <Servo.h>
const byte qmc5883l_mode_stby = 0x00;
const byte qmc5883l_mode_cont = 0x01;
const byte qmc5883l_odr_10hz = 0x00;
const byte qmc5883l_odr_50hz = 0x04;
const byte qmc5883l_odr_100hz = 0x08;
const byte qmc5883l_odr_200hz = 0x0C;
const byte qmc5883l_rng_2g = 0x00;
const byte qmc5883l_rng_8g = 0x10;
const byte qmc5883l_osr_512 = 0x00;
const byte qmc5883l_osr_256 = 0x40;
const byte qmc5883l_osr_128 = 0x80;
const byte qmc5883l_osr_64 = 0xC0;
QMC5883LCompass compass;           // communicate through i2c communication scl-A5 and sda-A4
SoftwareSerial gpsSerial(10, 11);  // 10 = TX and 11 = rx
AF_DCMotor m1(1);
AF_DCMotor m2(2);
AF_DCMotor m3(3);
AF_DCMotor m4(4);
int inp;
int speed = 120;
#define trigPin A0  // 12 or trigger pin to A0
#define echoPin A1  // 13 or trigger pin to A1
long duration;
int distance;
Servo servomotor;


//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);
  compass.init();
  compass.setCalibration(-155, 2072, -3057, -892, -1072, 1177);

  m1.setSpeed(speed);
  m2.setSpeed(speed);
  m3.setSpeed(speed);
  m4.setSpeed(speed);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  servomotor.attach(9);
  servomotor.write(90);
}

//**************************************

void loop() {
  double inlat = 12.9762332;
  double inlog = 79.1609433;

  double longitude = get_log();
  Serial.println(longitude, 7);

  double latitude = get_lat();
  Serial.println(latitude, 7);
  //delay(100);

  int direction = get_direction();
  Serial.println(direction);

  get_west();

  long dist = get_distance();
  Serial.println(dist);

  //this loops runs until the rover's longitude matches with the giveen destination longitude range
  while (!(longitude >= (inlog - 0.0002) && longitude <= (inlog + 0.0002))) {
    obstacle_avoid();
    if (inlog > longitude) {
      longitude = get_log();
      Serial.println(longitude, 7);
      Serial.println("going east");
      get_east();
    } else if (inlog < longitude) {
      longitude = get_log();
      Serial.println(longitude, 7);
      Serial.println("going west");
      get_west();
    }
    longitude = get_log();
  }


  //this loops runs until the rover's latitude matches with the giveen destination  latitude range
  while (!(latitude >= (inlat - 0.0002) && latitude <= (inlat + 0.0002))) {
    obstacle_avoid();

    if (inlat > latitude) {
      latitude = get_lat();
      Serial.println("going north");
      Serial.println(latitude, 7);
      get_north();

    } else if (inlat < latitude) {
      latitude = get_lat();
      Serial.println(latitude, 7);
      Serial.println("going south");
      get_south();
    }
    latitude = get_lat();
   } 
}

//**************************************

// code for avoiding the obstacle

void obstacle_avoid() {
  long distance_obj = get_distance();
  if (distance_obj <= 40) {
    motorstop();
    delay(200);
    back();
    delay(400);
    motorstop();
    delay(200);

    long distance_right = lookright();
    delay(100);
    long distance_left = lookleft();
    delay(100);
    Serial.println(distance_right);
    Serial.println(distance_left);

    if (distance_right >= distance_left) {
      Serial.println("mov rig");
      right();
      delay(900);
      front();
      delay(900);

    } else {
      Serial.println("mov lef");
      left();
      delay(900);
      front();
      delay(900);
    }
  }
}
//_________________________________________________________

// code for getting right distance for obstacle

long lookright() {
  servomotor.write(0);
  delay(500);
  long distancee = get_distance();
  delay(100);
  servomotor.write(90);
  return distancee;
}

//_________________________________________________________

// code for getting right distance for obstacle

long lookleft() {
  servomotor.write(180);
  delay(500);
  long distances = get_distance();
  delay(100);
  servomotor.write(90);
  return distances;
}

//________________________________________________________

//function to get the rover in west direction

void get_west() {
  unsigned long start_time = millis();    // Record the start time
  while (millis() - start_time < 1000) {  // Run for 1 seconds
    int direction = get_direction();
    if (direction == 0 ) {
      front();
      delay(500);
      return;
    }
    if (direction >= 1 && direction <= 9) {
      left();

    } else if (direction >= 10 && direction <= 15) {
      right();
    }
  }
  // If 1 seconds have passed, exit the function
  return;
}

//________________________________________________________________

//function to get the rover in east direction//

void get_east() {
  unsigned long start_time = millis();    // Record the start time
  while (millis() - start_time < 1000) {  // Run for 1 seconds
    int direction = get_direction();
    if (direction == 8 || direction == 7) {
      front();
      delay(500);
      return;
    }
    if (direction >= 9 && direction <= 15) {
      left();
      delay(50);
    } else if (direction >= 0 && direction <= 6) {
      right();
      delay(50);
    }
  }
  // If 1 seconds have passed, exit the function
  return;
}


//___________________________________________________________________

//function to get the rover in south direction//

void get_south() {
  unsigned long start_time = millis();    // Record the start time
  while (millis() - start_time < 1000) {  // Run for 2 seconds
    int direction = get_direction();
    if (direction == 13) {
      front();
      delay(500);
      return;
    }
    //Serial.println(direction);
    if (direction >= 3 && direction <= 12) {
      right();

    } else if (direction == 1 || direction == 2 || direction == 14 || direction == 15 || direction == 0) {
      left();
    }
  }
  // If 1 seconds have passed, exit the function
  return;
}

//____________________________________________________________________

//function to get the rover in north direction

void get_north() {
  unsigned long start_time = millis();    // Record the start time
  while (millis() - start_time < 1000) {  // Run for 2 seconds
    int direction = get_direction();
    if (direction == 2) {
      front();
      delay(500);
      return;
    }
    if (direction >= 3 && direction <= 11) {
      left();

    } else if (direction >= 12 && direction <= 15 || direction == 1 || direction == 0) {
      right();
    }
  }
  // If 1 seconds have passed, exit the function
  return;
}

//____________________________________________________________________________________________________________________//

//Code for getting  latitude

double get_lat() {
  while (gpsSerial.available()) {
    String gpsData = gpsSerial.readStringUntil('\n');
    //Serial.println(gpsData);

    // Check if the GPS data is a $GPGLL sentence
    if (gpsData.startsWith("$GPGLL")) {
      // Split the GPS data into individual data fields
      String dataFields[6];
      int fieldCount = 0;
      int startIndex = 0;
      for (int i = 0; i < gpsData.length(); i++) {
        if (gpsData.charAt(i) == ',') {
          dataFields[fieldCount] = gpsData.substring(startIndex, i);
          fieldCount++;
          startIndex = i + 1;
        }
      }
      dataFields[fieldCount] = gpsData.substring(startIndex, gpsData.length());

      // Extract latitude and longitude from the GPS data
      float latitude = dataFields[1].toFloat() / 100;
      float longitude = dataFields[3].toFloat() / 100;
      double lat_int_part, lat_dec_part;
      double log_int_part, log_dec_part;

      lat_dec_part = modf(latitude, &lat_int_part);
      log_dec_part = modf(longitude, &log_int_part);

      double total_latd = (lat_dec_part / 60) * 100;
      double total_logd = (log_dec_part / 60) * 100;


      double finallat = lat_int_part + total_latd;
      double finallog = log_int_part + total_logd;

      return (finallat);
    }
  }
}

//____________________________________________________

double get_log() {
  while (gpsSerial.available()) {
    String gpsData = gpsSerial.readStringUntil('\n');
    //Serial.println(gpsData);

    // Check if the GPS data is a $GPGLL sentence
    if (gpsData.startsWith("$GPGLL")) {
      // Split the GPS data into individual data fields
      String dataFields[6];
      int fieldCount = 0;
      int startIndex = 0;
      for (int i = 0; i < gpsData.length(); i++) {
        if (gpsData.charAt(i) == ',') {
          dataFields[fieldCount] = gpsData.substring(startIndex, i);
          fieldCount++;
          startIndex = i + 1;
        }
      }
      dataFields[fieldCount] = gpsData.substring(startIndex, gpsData.length());

      // Extract latitude and longitude from the GPS data
      float latitude = dataFields[1].toFloat() / 100;
      float longitude = dataFields[3].toFloat() / 100;
      double lat_int_part, lat_dec_part;
      double log_int_part, log_dec_part;

      lat_dec_part = modf(latitude, &lat_int_part);
      log_dec_part = modf(longitude, &log_int_part);

      double total_latd = (lat_dec_part / 60) * 100;
      double total_logd = (log_dec_part / 60) * 100;


      double finallat = lat_int_part + total_latd;
      double finallog = log_int_part + total_logd;


      return (finallog);
    }
  }
}
//______________________________________________________

// code for getting direction angle

double get_direction() {
  int x_value;
  int y_value;
  int z_value;
  int azimuth;
  byte bearing;
  char direction[strlen("NNE") + 1];
  char buffer[strlen("X=-99999 | Y=-99999 | Z=-99999 | A=259° | B=15 | D=NNE") + 1];

  compass.read();  // Read compass values via I2C

  x_value = compass.getX();
  y_value = compass.getY();
  z_value = compass.getZ();
  azimuth = compass.getAzimuth();  // Calculated from X and Y value
  bearing = compass.getBearing(azimuth);

  compass.getDirection(direction, azimuth);
  direction[3] = '\0';

  return (bearing);
}
//__________________________________________________

// code for ultra sonic sensor to get abstacle distance

long get_distance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  return (distance);
}
//__________________________________________________________

// code for movement for rover

void back() {
  m1.run(FORWARD);
  m2.run(FORWARD);
  m3.run(FORWARD);
  m4.run(FORWARD);
  delay(500);
}

void front() {
  m1.run(BACKWARD);
  m2.run(BACKWARD);
  m3.run(BACKWARD);
  m4.run(BACKWARD);
}

void right() {
  m1.run(BACKWARD);
  m2.run(BACKWARD);
  m3.run(FORWARD);
  m4.run(FORWARD);
}

void left() {
  m1.run(FORWARD);
  m2.run(FORWARD);
  m3.run(BACKWARD);
  m4.run(BACKWARD);
}

void motorstop() {
  m1.run(RELEASE);
  m2.run(RELEASE);
  m3.run(RELEASE);
  m4.run(RELEASE);
}