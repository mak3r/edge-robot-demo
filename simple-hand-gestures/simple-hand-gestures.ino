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
byte cur_pos[] = {0,0,0,0,0};

//chill macro
#define chill(pos,n) (pos/16*n)

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
  pwm.setPWMFreq(SERVO_FREQ);  // This is the maximum PWM frequency

  interval = agent.getRegister(gesture_hold) * 1000;
  interval = constrain(interval, 2000, 20000);
}

byte flip = 0;
void loop() {

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    pwm.wakeup();
    (flip) ? gesture() : relax();
    pwm.sleep();
    flip = !flip;
  }
  

}

void gesture() {
  for (int i=0; i<16; i++) {  
    next_gesture(g1_pinky+i, (byte)pinky );
    next_gesture(g1_ring+i, (byte)ring );
    next_gesture(g1_middle+i, (byte)middle );
    next_gesture(g1_pointer+i, (byte)pointer );
    next_gesture(g1_thumb+i, (byte)thumb );
    delay(100);
  }
}

void relax() {
  
  for (int i=16; i>=1; i--) {  
    pwm.writeMicroseconds((byte)pinky, chill(cur_pos[pinky],i) );
    pwm.writeMicroseconds((byte)ring, chill(cur_pos[ring],i) );
    pwm.writeMicroseconds((byte)middle, chill(cur_pos[middle],i) );
    pwm.writeMicroseconds((byte)pointer, chill(cur_pos[pointer],i) );
    pwm.writeMicroseconds((byte)thumb, chill(cur_pos[thumb],i) );

//    next_gesture(cur_pos[pinky]/i, (byte)pinky );
//    next_gesture(chill[i], (byte)ring );
//    next_gesture(chill[i], (byte)middle );
//    next_gesture(chill[i], (byte)pointer );
//    next_gesture(chill[i], (byte)thumb );
    delay(100);
  }
  cur_pos[pinky] = 16;
  cur_pos[ring] = 16;
  cur_pos[middle] = 16;
  cur_pos[pointer] = 16;
  cur_pos[thumb] = 16;
}

void next_gesture(gesture_offset off, finger f) {
    cur_pos[f] = agent.getRegister(off);
    pwm.writeMicroseconds((byte)f, calculate_uS(off));
}
