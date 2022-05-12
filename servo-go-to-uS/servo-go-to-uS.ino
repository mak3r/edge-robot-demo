/*
 Try this test sketch with the Servo library to see how your
 servo responds to different settings, type a position from
 544 to 2400 in the top of serial monitor and hit [ENTER],
 start at 1472 and work your way toward zero (544) 50 micros at
 a time, then toward 2400. 
*/
#include <Servo.h>
Servo servo;

void setup() {
  // initialize serial:
  Serial.begin(9600); //set serial monitor baud rate to match
  servo.writeMicroseconds(1472);
  servo.attach(9);
  prntIt();
}

void loop() {
  // if there's any serial available, read it:
  while (Serial.available() > 0) {

    // look for the next valid integer in the incoming serial stream:
    int pos = Serial.parseInt();
    pos = constrain(pos, 400, 2400);
    servo.writeMicroseconds(pos);
    prntIt();
    Serial.read(); //make sure to clear the buffer of \0
  }
}
void prntIt()
{
  Serial.print("  degrees = "); 
  Serial.print(servo.read());
  Serial.print("\t");
  Serial.print("microseconds =  ");
  Serial.println(servo.readMicroseconds());
}
