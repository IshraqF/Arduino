/*
 * Author: Farhan Ishraq
 * Purpose: The program takes input from the joystick module to set the speed and direction of the gear motors
 */

#include <SoftwareSerial.h>
SoftwareSerial BT(2, 3);

/* enable pins for right and left motors */
#define ENA 9
#define ENB 10

/* input pins to control the right and left motors */
#define IN1 4
#define IN2 5
#define IN3 6
#define IN4 7

/* stores x and y values from the joystick */
int xAxis = 140;
int yAxis = 140;

/* set the speed of the right and left motors*/ 
int mSpeedRight = 0;
int mSpeedLeft = 0;

/* maps the x values from 0-1023 to 0-255*/
int xMapped = 0;

/* function prototypes */
void readYaxis(int);
void readXaxis(int);

void setup() {
  /* set all pins as OUTPUT*/
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  /* defaul communication frequency */
  BT.begin(9600);
}

void loop() {
  /* Read the x and y values from mobile app */
  while (BT.available() >= 2) {
    xAxis = BT.read();
    delay(10);
    yAxis = BT.read();
  }
  delay(10);
  
  /* use the x and y values to set the speed and direction of the motors*/
  readYaxis(yAxis);
  readXaxis(xAxis);

  /* provent the motors from buzzing due to low speed */
  if (mSpeedLeft < 50) {
    mSpeedLeft = 0;
  }

  if (mSpeedRight < 50) {
    mSpeedRight = 0;
  }

  /* write the speed to the motors */
  analogWrite(ENA, mSpeedRight);
  analogWrite(ENB, mSpeedLeft);
}


/*
 * Purpose: checks the y values from the joystick to determine the forward and backward movement of the car
 * Param: yAxis -- y values from the joystick
 */
void readYaxis(int yAxis) {
  if (yAxis > 160) { // move backward
    /* for right motor */
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);

    /* for left motor */
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);

    /* map the speeds */
    mSpeedRight = map(yAxis, 140, 220, 0, 255);
    mSpeedLeft = map(yAxis, 140, 220, 0, 255);
  }
  else if (yAxis < 120) { // move forward
    /* for right motor */
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);

    /* for left motor */
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);

    /* map the speeds */
    mSpeedRight = map(yAxis, 140, 60, 0, 255);
    mSpeedLeft = map(yAxis, 140, 60, 0, 255);
  }
  else {
    /* joystick is idle */
    mSpeedRight = 0;
    mSpeedLeft = 0;
  }
}


/*
 * Purpose: checks the x values from the joystick to determine the left and right movement of the car
 * Param: xAxis -- x values from the joystick
 */
void readXaxis(int xAxis) {
  if (xAxis < 120) { // move left
    /* map the xAxis values */
    xMapped = map(xAxis, 140, 60, 0, 255);
    //xMapped = xAxis;
    /* set the speeds */
    mSpeedRight = mSpeedRight + xMapped;
    mSpeedLeft = mSpeedLeft - xMapped;

    /* keep speed within threshold */
    if (mSpeedRight > 255) {
      mSpeedRight = 255;
    }
    if (mSpeedLeft < 0) {
      mSpeedLeft = 0;
    }
  }
  else if (xAxis > 160) { // move right
    /* map the xAxis values */
    xMapped = map(xAxis, 140, 220, 0, 255);
    //xMapped = xAxis;
    /* set the speeds */
    mSpeedLeft = mSpeedLeft + xMapped;
    mSpeedRight = mSpeedRight - xMapped;

    /* keep within threshold */
    if (mSpeedLeft > 255) {
      mSpeedLeft = 255;
    }
    if (mSpeedRight < 0) {
      mSpeedRight = 0;
    }
  }
}
