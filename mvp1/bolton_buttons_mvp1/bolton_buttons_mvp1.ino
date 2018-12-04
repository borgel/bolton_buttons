#include <usb_keyboard.h>

void setup() {
  Serial.begin(9600);

  //TODO interrupt config

  // wait for USB enumeration
  delay(1000);
  Keyboard.print("a");
}

void loop() {
  //FIXME rm
  Serial.println("loop");
  delay(1000);

  //check for key flags, and send data
}
