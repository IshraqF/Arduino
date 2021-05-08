/*
 * Author: Farhan Ishraq
 * Purpose: The program takes input from the joystick module to set the speed and direction of the gear motors
 */

/* enable pins for right and left motors */
#define ENA 9
#define ENB 10

/* input pins to control the right and left motors */
#define IN1 4
#define IN2 5
#define IN3 6
#define IN4 7

/* analog pins to receive the x and y values from joystick*/
const int VRyPin = A1;
const int VRxPin = A0;

/* stores x and y values from the joystick */
int xAxis = 0;
int yAxis = 0;

/* set the speed of the right and left motors*/ 
int mSpeedRight = 0;
int mSpeedLeft = 0;

/* maps the y values from 0-1023 to 0-255*/
int yMapped = 0;

/* function prototypes */
void readXaxis(int);
void readYaxis(int);

void setup() {
  /* set all pins as OUTPUT*/
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  /* defaul communication frequency */
  Serial.begin(9600);
}

void loop() {
  /* read the x and y axis values */
  xAxis = analogRead(VRxPin);
  yAxis = analogRead(VRyPin);

  /* use the x and y values to set the speed and direction of the motors*/
  readXaxis(xAxis);
  readYaxis(yAxis);

  /* provent the motors from buzzing due to low speed */
  if (mSpeedLeft < 80) {
    mSpeedLeft = 0;
  }

  if (mSpeedRight < 80) {
    mSpeedRight = 0;
  }

  /* write the speed to the motors */
  analogWrite(ENA, mSpeedRight);
  analogWrite(ENB, mSpeedLeft);
}


/*
 * Purpose: checks the x values from the joystick to determine the forward and backward movement of the car
 * Param: xAxis -- x values from the joystick
 */
void readXaxis(int xAxis) {
  if (xAxis < 514) {
    /* for right motor */
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);

    /* for left motor */
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);

    /* map the speeds */
    mSpeedRight = map(xAxis, 514, 0, 0, 255);
    mSpeedLeft = map(xAxis, 514, 0, 0, 255);
  }
  else if (xAxis > 520) {
    /* for right motor */
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);

    /* for left motor */
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);

    /* map the speeds */
    mSpeedRight = map(xAxis, 520, 1023, 0, 255);
    mSpeedLeft = map(xAxis, 520, 1023, 0, 255);
  }
  else {
    /* joystick is idle */
    mSpeedRight = 0;
    mSpeedLeft = 0;
  }
}


/*
 * Purpose: checks the y values from the joystick to determine the left and right movement of the car
 * Param: yAxis -- y values from the joystick
 */
void readYaxis(int yAxis) {
  if (yAxis < 520) {
    /* map the yAxis values */
    yMapped = map(yAxis, 520, 0, 0, 255);

    /* set the speeds */
    mSpeedRight = mSpeedRight + yMapped;
    mSpeedLeft = mSpeedLeft - yMapped;

    /* keep speed within threshold */
    if (mSpeedRight > 255) {
      mSpeedRight = 255;
    }
    if (mSpeedLeft < 0) {
      mSpeedLeft = 0;
    }
  }
  else if (yAxis > 528) { 
    /* map the yAxis values */
    yMapped = map(yAxis, 528, 1023, 0, 255);

    /* set the speeds */
    mSpeedLeft = mSpeedLeft + yMapped;
    mSpeedRight = mSpeedRight - yMapped;

    /* keep within threshold */
    if (mSpeedLeft > 255) {
      mSpeedLeft = 255;
    }
    if (mSpeedRight < 0) {
      mSpeedRight = 0;
    }
  }
}
