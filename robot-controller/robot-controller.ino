#include <I2CSlaveMode.h>
#include <Adafruit_PWMServoDriver.h>

//Initialize arduino i2c for config data
I2CSlaveMode i2cSlaveMode = new I2CSlaveMode();
//Initialize Adafruit servo driver board to address 0x40 (the default)
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();


// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

// our servo # counter
uint8_t servonum = 0;

void setup() {

  Serial.begin(9600);
  Serial.println("\nRobot Controller");

  pwm.begin(); //This calls Wire.begin()
  pwm.setPWMFreq(SERVO_FREQ);  // This is the maximum PWM frequency

}



void loop() {
  // Drive each servo one at a time using setPWM()
  Serial.println(servonum);
  uint8_t count = 0;
  for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
    pwm.setPWM(servonum, 0, pulselen);
    count++;
  }
  Serial.print("Count of setPWM() calls: ");
  Serial.println(count);
  count = 0; 

  delay(500);
  for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
    pwm.setPWM(servonum, 0, pulselen);
  }

  delay(500);

  // Drive each servo one at a time using writeMicroseconds(), it's not precise due to calculation rounding!
  // The writeMicroseconds() function is used to mimic the Arduino Servo library writeMicroseconds() behavior. 
  for (uint16_t microsec = USMIN; microsec < USMAX; microsec++) {
    pwm.writeMicroseconds(servonum, microsec);
    count++;
  }
  Serial.print("Count of writeMicroseconds() calls: ");
  Serial.println(count);
  count = 0; 

  delay(500);
  for (uint16_t microsec = USMAX; microsec > USMIN; microsec--) {
    pwm.writeMicroseconds(servonum, microsec);
  }

  delay(500);

  servonum++;
  if (servonum > 7) servonum = 0; // Testing the first 8 servo channels
}
