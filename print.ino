#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

#define xDirPin 5
#define yDirPin 6

#define xStepPin 2
#define yStepPin 3

#define EnPin 8

#define touchsenserPin 9
// variable for reading the pushbutton status


String txtMsg = "";
bool state = false;
int gcodeRowIndex = 0;
int power = 500;
int delaytime = 0;

String inString = "";    // string to hold input


int cmdX = 0;
int cmdY = 0;
int nowX = 50;
int nowY = 50;

int cmdflag = 0;

int Xdir = 0;
int Ydir = 0;
void servo() {
  digitalWrite(EnPin, HIGH);
  myservo.write(100);
  while (!digitalRead(touchsenserPin)) {
        Serial.println("wait touch");
    delay(10);
  }
  delay(power);
  myservo.write(80);
  delay(power);
  myservo.write(90);
}

String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void moving(int cmdX , int cmdY) {
  while (cmdflag == 1) {
    if (cmdX > nowX) {
      digitalWrite(EnPin, LOW);
      nowX++;
      Serial.println(nowX);
      digitalWrite(xDirPin, HIGH);
      digitalWrite(xStepPin, LOW);
      delay(delaytime);
      digitalWrite(xStepPin, HIGH);
    } else if (cmdX < nowX) {
      digitalWrite(EnPin, LOW);
      nowX--;
      Serial.println(nowX);
      digitalWrite(xDirPin, LOW);
      digitalWrite(xStepPin, LOW);
      delay(delaytime);
      digitalWrite(xStepPin, HIGH);
      delay(delaytime);
    }

    if (cmdY > nowY) {
      digitalWrite(EnPin, LOW);
      nowY++;
      Serial.println(nowY);
      digitalWrite(yDirPin, HIGH);
      digitalWrite(yStepPin, LOW);
      delay(delaytime);
      digitalWrite(yStepPin, HIGH);
      delay(delaytime);
    } else if (cmdY < nowY) {
      digitalWrite(EnPin, LOW);
      nowY--;
      Serial.println(nowY);
      digitalWrite(yDirPin, LOW);
      digitalWrite(yStepPin, LOW);
      delay(delaytime);
      digitalWrite(yStepPin, HIGH);
      delay(delaytime);
    }
    if (cmdX == nowX && cmdY == nowY) {
      if (cmdflag == 1) {
        servo();
        Serial.print("OK");
        digitalWrite(EnPin, HIGH);
        cmdflag = 0;
      }
    }
  }

}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(xDirPin, OUTPUT);
  pinMode(yDirPin, OUTPUT);
  pinMode(xStepPin, OUTPUT);
  pinMode(yStepPin, OUTPUT);
  pinMode(EnPin, OUTPUT);
  pinMode(touchsenserPin, INPUT);

  digitalWrite(EnPin, HIGH);
  while (1) {//for debug
    cmdflag=1;
    moving(100, 100);
    delay(1000);
    cmdflag=1;
    moving(0, 0);
  }
  myservo.attach(11);  // attaches the servo on pin 9 to the servo object
  while (!digitalRead(touchsenserPin)) {
        Serial.println("pls touch sensor");
    delay(10);
  }
}

void loop() {
  digitalWrite(EnPin, HIGH);

  while (Serial.available() > 0) {
    char inChar = Serial.read();

    if (inChar == 'c') {
      txtMsg = "";
    }
    else {
      txtMsg += inChar;
    }
    if (inChar == 'd') {
      Serial.print("ok");
      delay(50);
      Serial.print("end");
      state = true;
    }

  }
  if (state) {

    String gcode = getValue(txtMsg, ';', gcodeRowIndex);

    if (gcode == "end") {

      gcodeRowIndex = 0;
      txtMsg = "";
      state = false;
    }
    else {
      int  x = getValue(gcode, ',', 0).toInt();
      int  y = getValue(gcode, ',', 1).toInt();
      if (gcodeRowIndex == 0) {
        power = x;
        delaytime = y;

      } else {
        cmdflag=1;
        moving(x, y);
      }
      ++gcodeRowIndex;
    }

  }
}
