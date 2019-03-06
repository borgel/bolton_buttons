
#include <i2c_t3.h>
#include "TCA9555.h"
#include <Encoder.h>

// bits a2, a1, a0
TCA9555 tca9555(1,0,0);

uint16_t lastButtonMask = 0x0;

//teensy pin 13 LED (active low)
//expander P17 status LED (active low)

// all the knobs
static Encoder knob1(11, 10);
static Encoder knob2(9, 8);
static Encoder knob3(7, 6);
static Encoder knob4(5, 4);

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  Wire1.begin();

  // IO expander interrupt port
  pinMode(2, INPUT);
  attachInterrupt(2, buttonChangeISR, FALLING);

  // setup IO expander
  // each port all inputs, except last bit of second
  // port 0. bit set to 1 is input
  tca9555.setPortDirection(0, 0xFF);
  tca9555.setPortDirection(1, ~((byte)(1 << 7)));
  
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
  Serial.println(lastButtonMask, HEX);

  tca9555.setOutputStates(1, t << 7);

  Serial.printf("%d %d %d %d\n",
    knob1.read(),
    knob2.read(),
    knob3.read(),
    knob4.read()
    );
  
  digitalWrite(13, t);
  t = !t;
  
  delay(1000);
}

void buttonChangeISR() {
  lastButtonMask = 0x7FFF & tca9555.getInputStates();
}
