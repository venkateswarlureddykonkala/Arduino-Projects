AF_DCMotor m1(1);
AF_DCMotor m2(2);
AF_DCMotor m3(3);
AF_DCMotor m4(4);
int inp;
int Speeed = 225;
void setup() {
  Serial.begin(9600);
}
void loop() {
  if (Serial.available() > 0) {
    inp = Serial.read();
    Serial.println(inp);

    if (inp == 51) {
      front();
    }
    if (inp == 52) {
      back();
    }
    if (inp == 49) {
      left();
    }
    if (inp == 50) {
      right();
    }
    if (inp == 114) {
      topright();
    }
    if (inp == 102) {
      topleft();
    }
    if (inp == 108) {
      bottomright();
    }
    if (inp == 98) {
      bottomleft();
    }
  }
}
void front() {
  m1.setSpeed(Speeed);
  m1.run(FORWARD);
  m2.setSpeed(Speeed);
  m2.run(FORWARD);
  m3.setSpeed(Speeed);
  m3.run(FORWARD);
  m4.setSpeed(Speeed);
  m4.run(FORWARD);
}
void back() {
  m1.setSpeed(Speeed);
  m1.run(BACKWARD);
  m2.setSpeed(Speeed);
  m2.run(BACKWARD);
  m3.setSpeed(Speeed);
  m3.run(BACKWARD);
  m4.setSpeed(Speeed);
  m4.run(BACKWARD);
}
void left() {
  m1.setSpeed(Speeed);
  m1.run(BACKWARD);
  m2.setSpeed(Speeed);
  m2.run(BACKWARD);
  m3.setSpeed(Speeed);
  m3.run(FORWARD);
  m4.setSpeed(Speeed);
  m4.run(FORWARD);
}
void right() {
  m1.setSpeed(Speeed);
  m1.run(FORWARD);
  m2.setSpeed(Speeed);
  m2.run(FORWARD);
  m3.setSpeed(Speeed);
  m3.run(BACKWARD);
  m4.setSpeed(Speeed);
  m4.run(BACKWARD);
}
void topright() {
  m1.setSpeed(Speeed);
  m1.run(FORWARD);
  m2.setSpeed(Speeed);
  m2.run(FORWARD);
  m3.setSpeed(Speeed / 3);
  m3.run(FORWARD);
  m4.setSpeed(Speeed / 3);
  m4.run(FORWARD);
}
void topleft() {
  m1.setSpeed(Speeed / 3);
  m1.run(FORWARD);
  m2.setSpeed(Speeed / 3);
  m2.run(FORWARD);
  m3.setSpeed(Speeed);
  m3.run(FORWARD);
  m4.setSpeed(Speeed);
  m4.run(FORWARD);
}
void bottomright() {
  m1.setSpeed(Speeed);
  m1.run(BACKWARD);
  m2.setSpeed(Speeed);
  m2.run(BACKWARD);
  m3.setSpeed(Speeed / 3);
  m3.run(BACKWARD);
  m4.setSpeed(Speeed / 3);
  m4.run(BACKWARD);
}
void bottomleft() {
  m1.setSpeed(Speeed / 3);
  m1.run(BACKWARD);
  m2.setSpeed(Speeed / 3);
  m2.run(BACKWARD);
  m3.setSpeed(Speeed);
  m3.run(BACKWARD);
  m4.setSpeed(Speeed);
  m4.run(BACKWARD);
}