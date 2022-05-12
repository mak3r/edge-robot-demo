#include <I2CSlaveMode.h>

I2CSlaveMode i2cSlaveMode = I2CSlaveMode(0x33);

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  Serial.println("Debug I2CSlaveMode");
  Serial.println(i2cSlaveMode.getAddress());
  Serial.end();
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(10);
  i2cSlaveMode.resetIfRequested();
}
