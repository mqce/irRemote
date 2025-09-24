#include <Arduino.h>
#include "PinDefinitionsAndMore.h"
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


void setup() {
  Serial.begin(115200);

  // Start the receiver and if not 3. parameter specified, take LED_BUILTIN pin from the internal boards definition as default feedback LED
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

  /*
  Serial.print(F("Ready to receive IR signals of protocols: "));
  printActiveIRProtocols(&Serial);
  Serial.println(F("at pin " STR(IR_RECEIVE_PIN)));
  */
}

void loop() {
  /*
  * Check if received data is available and if yes, try to decode it.
  * Decoded result is in the IrReceiver.decodedIRData structure.
  *
  * E.g. command is in IrReceiver.decodedIRData.command
  * address is in command is in IrReceiver.decodedIRData.address
  * and up to 32 bit raw data in IrReceiver.decodedIRData.decodedRawData
  */
  if (IrReceiver.decode()) {

      /*
      * Print a summary of received data
      */
      if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
          Serial.println(F("Received noise or an unknown (or not yet enabled) protocol"));
          // We have an unknown protocol here, print extended info
          IrReceiver.printIRResultRawFormatted(&Serial, true);

          IrReceiver.resume(); // Do it here, to preserve raw data for printing with printIRResultRawFormatted()
      } else {
          IrReceiver.resume(); // Early enable receiving of the next IR frame

          IrReceiver.printIRResultShort(&Serial);
          IrReceiver.printIRSendUsage(&Serial);
      }
      Serial.println();

      if(IrReceiver.decodedIRData.command == 0xD8) {
        Serial.println(F("Received command 0xD8."));
      }
      
      /*
      * Finally, check the received data and perform actions according to the received command

      if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) {
          Serial.println(F("Repeat received. Here you can repeat the same action as before."));
      } else {
          if (IrReceiver.decodedIRData.command == 0x10) {
              Serial.println(F("Received command 0x10."));
              // do something
          } else if (IrReceiver.decodedIRData.command == 0x11) {
              Serial.println(F("Received command 0x11."));
              // do something else
          }
      }
      */
  }
}