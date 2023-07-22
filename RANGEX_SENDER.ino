#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
WiFiClient client;

const char* ssid = "Venky";
const char* password = "987654321";

const char* serverName = "http://ROVER.000webhostapp.com//post-data.php"; // "http://pushpakthokala.000webhostapp.com//post-data.php";

String apiKeyValue = "12345";

int x1 = 16;  //d0
int y1v = 5;  //d1
int x2 = 4;   //d2
int y2 = 0;   //d3
long val1, val2, val3, val4, val5;

void setup() {
  pinMode(A0, INPUT);
  pinMode(x1, INPUT);
  pinMode(y1v, INPUT);
  pinMode(x2, INPUT);
  pinMode(y2, INPUT);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to the Wifi Network");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi is Connected at this IP Address : ");
  Serial.println(WiFi.localIP());
}

void loop() {
  val1 = analogRead(A0);
  val2 = digitalRead(x1);
  val3 = digitalRead(y1v);
  val4 = digitalRead(x2);
  val5 = digitalRead(y2);
 
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(client, serverName);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    String httpRequestData = "api_key=" + apiKeyValue + "&value1=" + val1
                             + "&value2=" + val2 + "&value3=" + val3
                             + "&value4=" + val4 + "&value5=" + val5 + "";
    //String httpRequestData ="api_key=#54321&SensorData=distance sensor&LocationData=Aicpecf-office&value1=NULL&value2=NULL&value3=NULL";
    Serial.print("httpRequestData: ");
    Serial.println(httpRequestData);
    int httpResponseCode = http.POST(httpRequestData);
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
  delay(1000);
}