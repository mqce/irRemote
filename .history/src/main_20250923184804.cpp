#include <Arduino.h>
// #include "PinDefinitionsAndMore.h"
#include <IRremote.hpp> 
/*
秋月電子 オプトサプライ赤外線リモコン
[protocol]
NEC
[Address]
0x10
[command]
P:0xD8
A:0xF8
B:0x78
C:0x58
↖:0xB1
↑:0xA0
↗:0x21
←:0x10
-:0x20
→:0x80
↙:0x11
↓:0x0
↘:0x81
 */

#define IR_RECEIVE_PIN 2


void setup() {
  Serial.begin(115200);
  IrReceiver.begin(IR_RECEIVE_PIN, false);
}

void loop() {
  if (IrReceiver.decode()) {
    IrReceiver.resume();
    if(IrReceiver.decodedIRData.command == 0xD8) {
      Serial.println(F("Received command 0xD8."));
    }
  }
}