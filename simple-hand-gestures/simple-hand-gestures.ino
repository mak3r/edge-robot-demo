#include <I2CSlaveMode.h>
#include <Adafruit_PWMServoDriver.h>

//Initialize arduino i2c for config data
I2CSlaveMode agent = new I2CSlaveMode();
//Initialize Adafruit servo driver board to address 0x40 (the default)
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates
unsigned long previousMillis = 0;
unsigned long interval = 5000;

enum finger {
  pinky = 0,
  ring,
  middle,
  pointer,
  thumb
};

//To hold the current position of each finger
int cur_pos[] = {0x400,0x400,0x400,0x400,0x400};

// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
// Tweak them by setting values in EEPROM registers
// Min starts at about 600 microseconds 
// Max starts at about 2400 microseconds 
enum uS_offset {
  gesture_hold = 0x04,//Register with a multiplier for number of seconds to hold the gesture 
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
  pwm.sleep();
  pwm.setPWMFreq(SERVO_FREQ);  // This is the maximum PWM frequency

  interval = agent.getRegister(gesture_hold) * 1000;
  // Arbitrarilly decided that the interval should not be less than 2 seconds 
  // or more than 20 seconds
  interval = constrain(interval, 2000, 20000);
}

byte flip = 0;
void loop() {

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    pwm.wakeup();
    (flip) ? gesture() : relax();
    flip = !flip;
  }
  pwm.sleep();
  

}

void gesture() {
  for (int i=0; i<16; i++) {  
    next_gesture(g1_pinky+i, (byte)pinky );
    next_gesture(g1_ring+i, (byte)ring );
    next_gesture(g1_middle+i, (byte)middle );
    next_gesture(g1_pointer+i, (byte)pointer );
    next_gesture(g1_thumb+i, (byte)thumb );
    delay(50);
  }
}

#define step_range(high, low) (abs(high-low))
#define new_pos(high, low, step) ( (step==0) ? high : (step_range(high,low)/step)+low )

void relax() {

  int pos_min = get_double_reg(uS_min);
  
  for (int i=0; i<16; i++) {  
    cur_pos[pinky] = new_pos(cur_pos[pinky], pos_min, i);
    pwm.writeMicroseconds((byte)pinky, cur_pos[pinky]);
    cur_pos[ring] = new_pos(cur_pos[ring], pos_min, i);
    pwm.writeMicroseconds((byte)ring, cur_pos[ring] );
    cur_pos[middle] = new_pos(cur_pos[middle], pos_min, i);
    pwm.writeMicroseconds((byte)middle, cur_pos[middle] );
    cur_pos[pointer] = new_pos(cur_pos[pointer], pos_min, i);
    pwm.writeMicroseconds((byte)pointer, cur_pos[pointer] );
    cur_pos[thumb] = new_pos(cur_pos[thumb], pos_min, i);
    pwm.writeMicroseconds((byte)thumb, cur_pos[thumb] );
    delay(50);
  }
}

void next_gesture(gesture_offset off, finger f) {
    cur_pos[f] = constrain(calculate_uS(off), get_double_reg(uS_min), get_double_reg(uS_max));
    pwm.writeMicroseconds((byte)f, cur_pos[f]);
}
