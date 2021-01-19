
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
String inString = "";    // string to hold input


int cmdX = 0;
int cmdY = 0;
int nowX = 50;
int nowY = 50;
int power=500;
int delaytime=0;

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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(xDirPin, OUTPUT);
  pinMode(yDirPin, OUTPUT);
  pinMode(xStepPin, OUTPUT);
  pinMode(yStepPin, OUTPUT);
  pinMode(EnPin, OUTPUT);
  pinMode(touchsenserPin, INPUT);

  digitalWrite(EnPin, HIGH);
  myservo.attach(11);  // attaches the servo on pin 9 to the servo object
  while (!digitalRead(touchsenserPin)) {
    Serial.println("pls touch sensor");
    delay(10);
  }
  //while (!Serial) {
  //  ; // wait for serial port to connect. Needed for native USB port only
  // }
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(EnPin, HIGH);
  while (Serial.available() > 0) {
    char inChar = Serial.read();
    txtMsg += inChar;
  }
  // print the message and a notice if it's changed:
  if (txtMsg.length() >= 11) {
    
    if (txtMsg[0] == '(' && txtMsg[8] == ')') {
      cmdflag = 1;
      Serial.println(txtMsg);
      Serial.println(txtMsg.length());
      Serial.println(txtMsg[1]);
      inString += txtMsg[1];
      inString += txtMsg[2];
      inString += txtMsg[3];
      cmdX = inString.toInt();
      inString = "";
      inString += txtMsg[5];
      inString += txtMsg[6];
      inString += txtMsg[7];
      cmdY = inString.toInt();
      inString = "";
      Serial.print("cmdX:");
      Serial.println(cmdX);
      Serial.print("cmdY:");
      Serial.println(cmdY);
    }else if (txtMsg[0] == 'P') {
      //cmdflag = 1;
      Serial.println(txtMsg);
      Serial.println(txtMsg.length());
      Serial.println(txtMsg[1]);
      inString += txtMsg[1];
      inString += txtMsg[2];
      inString += txtMsg[3];
      power = inString.toInt();
      inString = "";
      inString += txtMsg[5];
      inString += txtMsg[6];
      inString += txtMsg[7];
      //cmdY = inString.toInt();
      inString = "";
      Serial.print("power:");
      Serial.println(power);
    }else if (txtMsg[0] == 'D') {
      //cmdflag = 1;
      Serial.println(txtMsg);
      Serial.println(txtMsg.length());
      Serial.println(txtMsg[1]);
      inString += txtMsg[1];
      inString += txtMsg[2];
      inString += txtMsg[3];
      delaytime = inString.toInt();
      inString = "";
      inString += txtMsg[5];
      inString += txtMsg[6];
      inString += txtMsg[7];
      //cmdY = inString.toInt();
      inString = "";
      Serial.print("delaytime:");
      Serial.println(delaytime);
    }
    txtMsg = "";
  }
  if (cmdX > nowX) {
    digitalWrite(EnPin, LOW);
    Serial.println(nowX++);
    digitalWrite(xDirPin, HIGH);

    digitalWrite(xStepPin, LOW);
    delay(delaytime);
    digitalWrite(xStepPin, HIGH);

  } else if (cmdX < nowX) {
    digitalWrite(EnPin, LOW);
    Serial.println(nowX--);
    digitalWrite(xDirPin, LOW);
    digitalWrite(xStepPin, LOW);
    delay(delaytime);
    digitalWrite(xStepPin, HIGH);

  } else {

  }
  if (cmdY > nowY) {
    digitalWrite(EnPin, LOW);
    Serial.println(nowY++);
    digitalWrite(yDirPin, HIGH);
    digitalWrite(yStepPin, LOW);
    delay(delaytime);
    digitalWrite(yStepPin, HIGH);
    delay(delaytime);
  } else if (cmdY < nowY) {
    digitalWrite(EnPin, LOW);
    Serial.println(nowY--);
    digitalWrite(yDirPin, LOW);
    digitalWrite(yStepPin, LOW);
    delay(delaytime);
    digitalWrite(yStepPin, HIGH);

  } else {

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
