#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include <Servo.h>


const char* ssid = "Venky";
const char* password = "987654321";
String serverName = "http://ROVER.000webhostapp.com//read-data.php"; //"http://pushpakthokala.000webhostapp.com//read-data.php";
//#define PWM_L 16
#define IN1_L 16
#define IN2_L 4
//#define PWM_R 0
#define IN1_R 0
#define IN2_R 2
Servo servo;

void setMotor(int fdir, int in1_L, int in2_L, int in1_R, int in2_R) {
  //digitalWrite(pwm_L, 1);
  //digitalWrite(pwm_R, 1);
  if (fdir == 8) {
    digitalWrite(in1_L, HIGH);
    digitalWrite(in2_L, LOW);
    digitalWrite(in1_R, HIGH);
    digitalWrite(in2_R, LOW);
    Serial.println("front");
  } else if (fdir == 6) {
    digitalWrite(in1_L, HIGH);
    digitalWrite(in2_L, LOW);
    digitalWrite(in1_R, LOW);
    digitalWrite(in2_R, HIGH);
    Serial.println("right");
  } else if (fdir == 4) {
    digitalWrite(in1_L, LOW);
    digitalWrite(in2_L, HIGH);
    digitalWrite(in1_R, HIGH);
    digitalWrite(in2_R, LOW);
    Serial.println("left");
  } else if (fdir == 2) {
    digitalWrite(in1_L, LOW);
    digitalWrite(in2_L, HIGH);
    digitalWrite(in1_R, LOW);
    digitalWrite(in2_R, HIGH);
    Serial.println("back");
  } else if (fdir == 5 ) {
    digitalWrite(in1_L, LOW);
    digitalWrite(in2_L, LOW);
    digitalWrite(in1_R, LOW);
    digitalWrite(in2_R, LOW);
    Serial.println("stop");
  }
}

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  //pinMode(PWM_L, OUTPUT);
  pinMode(IN1_L, OUTPUT);
  pinMode(IN2_L, OUTPUT);
 // pinMode(PWM_R, OUTPUT);
  pinMode(IN1_R, OUTPUT);
  pinMode(IN2_R, OUTPUT);
  servo.attach(14); 
  servo.write(0);
}

void loop() {

  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;
    String serverPath = serverName;
    http.begin(client, serverPath.c_str());
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      StaticJsonDocument<192> doc;
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);

      DeserializationError error = deserializeJson(doc, payload);

      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        //return;
      }

      const char* id = doc["id"];
      const char* value1 = doc["value1"];
      const char* value2 = doc["value2"];
      const char* value3 = doc["value3"];
      const char* value4 = doc["value4"];
      const char* value5 = doc["value5"];

      int val0 = atoi(id);
      int val1 = atoi(value1);
      int val2 = atoi(value2);
      int val3 = atoi(value3);
      int val4 = atoi(value4);
      int val5 = atoi(value5);
      Serial.println(val0);
      Serial.println(val1);
      Serial.println(val2);
      Serial.println(val3);
      Serial.println(val4);
      Serial.println(val5);
      int servoValue = map(val1, 10, 900, 0, 360);
      servo.write(servoValue);
      int fdir;
      if (val2 == 0 && val3 == 0 && val4 == 0 && val5 == 1) {
        fdir = 5;
      } else if (val2 == 0 && val3 == 0 && val4 == 0 && val5 == 1) {
        fdir = 8;
      } else if (val2 == 0 && val3 == 1 && val4 == 0 && val5 == 0) {
        fdir = 2;
      } else if (val2 == 0 && val3 == 0 && val4 == 1 && val5 == 1) {
        fdir = 4;
      } else if (val2 == 1 && val3 == 0 && val4 == 0 && val5 == 1) {
        fdir = 6;
      }
      setMotor(fdir, IN1_L, IN2_L, IN1_R, IN2_R);
      delay(1);

    } else {
      Serial.println("Error code: " + String(httpResponseCode));
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}
