#include <I2CSlaveMode.h>
#include <Adafruit_PWMServoDriver.h>

//Initialize arduino i2c for config data
I2CSlaveMode agent = new I2CSlaveMode();
//Initialize Adafruit servo driver board to address 0x40 (the default)
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

enum finger {
  pinky = 0,
  ring,
  middle,
  pointer,
  thumb
};

// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
// Tweak them by setting values in EEPROM registers
// Min starts at about 600 microseconds 
// Max starts at about 2400 microseconds 
enum uS_offset {
  uS_divisor = 0x05, //Register holding the number of microsecond offsets possible
  uS_min = 0x06,     //Register holding the minimum microsecond length
  uS_max = 0x08,     //Register holding the maximum microsecond length
};

//Start registers for gestures
// Gestures are a sequence of 16 bytes
// each byte represents a servo offset 
enum gesture_offset {
  g1_pinky = 0x10,
  g1_ring = 0x20,
  g1_middle = 0x30,
  g1_pointer = 0x40,
  g1_thumb = 0x50,

  g2_pinky = 0x60,
  g2_ring = 0x70,
  g2_middle = 0x80,
  g2_pointer = 0x90,
  g2_thumb = 0xA0,

  g3_pinky = 0xB0,
  g3_ring = 0xC0,
  g3_middle = 0xD0,
  g3_pointer = 0xE0,
  g3_thumb = 0xF0,
} ;


// our servo # counter
uint8_t servonum = (uint8_t)pinky;

// Initialize the servo microseconds variables
uint16_t cur_uS = 1;
uint16_t next_uS = cur_uS;

uint16_t get_double_reg(uint8_t reg) {
  uint16_t value = agent.getRegister(reg);
  value <<= 8;
  value |= agent.getRegister(reg+1);
  return value;
}

uint16_t calculate_uS(gesture_offset gs) {
  uint16_t uS_min_local = get_double_reg(uS_min);
  uint16_t range = get_double_reg(uS_max) - uS_min_local;  
  return uS_min_local + ( ((float)agent.getRegister(gs)/(uint8_t)agent.getRegister(uS_divisor)) * range);
}



void setup() {

  Serial.begin(9600);
  Serial.println("\nRobot Controller");
  Serial.print("I2C address:");
  Serial.println(agent.getAddress());

  pwm.begin(); //This calls Wire.begin()
  pwm.setPWMFreq(SERVO_FREQ);  // This is the maximum PWM frequency

}

void loop() {

  //Registers hold values between 0 and 255
  next_uS = calculate_uS(g1_pinky);
  if ( next_uS != cur_uS ) {
    Serial.print("cur_uS: ");
    Serial.println(cur_uS);
    Serial.print("next_uS: ");
    Serial.println(next_uS);
    Serial.print("Microseconds [min, max]:[");
    Serial.print(get_double_reg(uS_min));
    Serial.print(",");
    Serial.print(get_double_reg(uS_max));
    Serial.println("]");
    Serial.flush();
    
    pwm.writeMicroseconds(servonum, next_uS);
    cur_uS = next_uS;
  }

  delay(500);

}
