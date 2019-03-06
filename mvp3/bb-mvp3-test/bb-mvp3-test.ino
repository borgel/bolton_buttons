
#include <i2c_t3.h>
#include "TCA9555.h"

// bits a2, a1, a0
TCA9555 tca9555(1,0,0);

//teensy pin 13 LED (active low)
//expander P17 status LED (active low)

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  Wire1.begin();

  // IO expander interrupt port
  pinMode(2, INPUT);

  // setup IO expander
  // each port all inputs, except last bit of second
  // port 0. bit set to 1 is input
  tca9555.setPortDirection(0, 0xFF);
  tca9555.setOutputStates(0, 0xFF);
  
  //tca9555.setPortDirection(1, ~((byte)(1 << 7)));
  
  // wait for USB enumeration
  delay(2000);
  Serial.begin(9600);
  Serial.println("Booted...");

  Serial.println("Done with setup");
  digitalWrite(13, HIGH);
}

bool t = false;
void loop() {
  //port 0
  Serial.println(tca9555.getInputStates(0), HEX);
  delay(1000);

  tca9555.setOutputStates(1, t << 7);
  
  digitalWrite(13, t);
  t = !t;
}
