#include <Servo.h>

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
#define debug 1
#define xDirPin 5
#define yDirPin 6

#define xStepPin 2
#define yStepPin 3

#define EnPin 8

#define xDefineDir 0
#define yDefineDir 1

#define touchsenserPin 9

#define laserPin 11
// variable for reading the pushbutton status


String txtMsg = "";
bool state = false;
int gcodeRowIndex = 0;
int power = 50;
int delaytime = 2;
String inString = "";    // string to hold input


long int cmdX = 0;
long int cmdY = 0;
long int nowX = 0;
long int nowY = 0;

int cmdflag = 0;

int Xdir = 0;
int Ydir = 0;
void servo() {
  digitalWrite(EnPin, HIGH);
  myservo.write(155 - power);
  /*
    while (!digitalRead(touchsenserPin)) {
        if(debug>=2)Serial.println("wait touch");
    delay(10);
    }
  */
  delay(100 + (power * 5));
  myservo.write(180);
  delay(100 + (power * 5));
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
void moveMotor(int motorNum, int motorDir, int step_count) {
  digitalWrite(EnPin, LOW);
  switch (motorNum) {
    case 1:
      while (step_count) {
        if (motorDir == 0) {
          digitalWrite(xDirPin, !xDefineDir);
          digitalWrite(xStepPin, LOW);
          delay(delaytime);
          digitalWrite(xStepPin, HIGH);
        } else {
          digitalWrite(xDirPin, xDefineDir);
          digitalWrite(xStepPin, LOW);
          delay(delaytime);
          digitalWrite(xStepPin, HIGH);
        }
        if (step_count >= 1) {
          step_count--;
        }
      }
      break;
    case 2:
      while (step_count) {
        if (motorDir == 0) {
          digitalWrite(yDirPin, !yDefineDir);
          digitalWrite(yStepPin, LOW);
          delay(delaytime);
          digitalWrite(yStepPin, HIGH);
        } else {
          digitalWrite(yDirPin, yDefineDir);
          digitalWrite(yStepPin, LOW);
          delay(delaytime);
          digitalWrite(yStepPin, HIGH);
        }
        if (step_count >= 1) {
          step_count--;
        }
      }
      break;

  }
}
void moving(long int cmdX , long int cmdY) {
  int movingFlag = 0;
  while (cmdflag == 1) {
    if (cmdX > nowX) {
      movingFlag = 1;
      digitalWrite(EnPin, LOW);
      nowX++;
      if (debug)Serial.println(nowX);
      digitalWrite(xDirPin, !xDefineDir);
      digitalWrite(xStepPin, LOW);
      delay(delaytime);
      digitalWrite(xStepPin, HIGH);
      //delay(delaytime);
    } else if (cmdX < nowX) {
      movingFlag = 1;
      digitalWrite(EnPin, LOW);
      nowX--;
      if (debug)Serial.println(nowX);
      digitalWrite(xDirPin, xDefineDir);
      digitalWrite(xStepPin, LOW);
      delay(delaytime);
      digitalWrite(xStepPin, HIGH);
      //delay(delaytime);
    }
    if (cmdY > nowY) {
      movingFlag = 1;
      digitalWrite(EnPin, LOW);
      nowY++;
      if (debug)Serial.println(nowY);
      digitalWrite(yDirPin, !yDefineDir);
      digitalWrite(yStepPin, LOW);
      delay(delaytime);
      digitalWrite(yStepPin, HIGH);
      //delay(delaytime);
    } else if (cmdY < nowY) {
      movingFlag = 1;
      digitalWrite(EnPin, LOW);
      nowY--;
      if (debug)Serial.println(nowY);
      digitalWrite(yDirPin, yDefineDir);
      digitalWrite(yStepPin, LOW);
      delay(delaytime);
      digitalWrite(yStepPin, HIGH);
      //delay(delaytime);
    }
    digitalWrite(EnPin, HIGH);
    if (cmdX == nowX && cmdY == nowY) {
      if (cmdflag == 1) {
        delay(150);
        if (movingFlag == 1)servo();
        delay(50);
        //if(debug)Serial.print("OK");
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
  pinMode(laserPin, OUTPUT);
  digitalWrite(laserPin, LOW);
  digitalWrite(EnPin, HIGH);
  myservo.attach(11);  // attaches the servo on pin 9 to the servo object
  delay(200);
  myservo.write(180);
  delay(1000);
  myservo.write(140);
  delay(1000);
  myservo.write(180);

  int x = 0;
  while (0) {
    myservo.write(0);
    delay(1000);
    myservo.write(180);
    delay(1000);
  }

  //極限測試
  //cmdflag=1;
  //moving(350, 0);
  //moving(0, 95000);

  if (0) {//for debug
    cmdflag = 1;
    moving(140, 120 * 50);
    delay(10000);
    cmdflag = 1;
    moving(0, 0);
    delay(10000);


  }
  /*
    while (!digitalRead(touchsenserPin)) {
        if(debug>=2)Serial.println("pls touch sensor");
    delay(10);
    }
  */
}

void loop() {

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
    if (inChar == '+') {
      moveMotor(1, 0, 1);
      digitalWrite(EnPin, HIGH);
    }
    if (inChar == '-') {
      moveMotor(1, 1, 1);
      digitalWrite(EnPin, HIGH);
    }
    if (inChar == '&') {
      moveMotor(2, 0, 50);
      digitalWrite(EnPin, HIGH);
    }
    if (inChar == '*') {
      moveMotor(2, 1, 50);
      digitalWrite(EnPin, HIGH);
    }


  }
  //Serial.print(state);
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
        //長300 寬100
        x = map(x, 0, 300, 0, 6000);
        y - map(y, 0, 80, 0, 140);
        cmdflag = 1;
        moving(y, x);
      }
      ++gcodeRowIndex;
    }

  }
}
