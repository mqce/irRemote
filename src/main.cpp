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
↖:0xB1 3
↑:0xA0 4
↗:0x21 5
←:0x10 6
-:0x20 7
→:0x80 8
↙:0x11 9
↓:0x0 10
↘:0x81 11
 */

#include <Arduino.h>
#include <IRremote.hpp> 
#include <array>

#define IR_RECEIVE_PIN 2

struct CommandPinMap {
  uint8_t command; // IR command code
  uint8_t pin;     // Arduino digital pin number to toggle
};

// Mapping based on lines 15-23
const CommandPinMap commandPinMappings[] = {
  {0xB1, 3}, // ↖
  {0xA0, 4}, // ↑
  {0x21, 5}, // ↗
  {0x10, 6}, // ←
  {0x20, 7}, // -
  {0x80, 8}, // →
  {0x11, 9}, // ↙
  {0x00, 10}, // ↓
  {0x81, 11}  // ↘
};

constexpr size_t NUM_PINS = sizeof(commandPinMappings) / sizeof(CommandPinMap);
bool pinStates[NUM_PINS] = {false}; // false = LOW, true = HIGH
unsigned long lastCommandMillis = 0; // timestamp of last accepted command

// Helper to write pin state from array
inline void applyPinState(size_t idx) {
  digitalWrite(commandPinMappings[idx].pin, pinStates[idx] ? HIGH : LOW);
}

void setup() {
  Serial.begin(115200);
  // Initialize IR receiver
  IrReceiver.begin(IR_RECEIVE_PIN, false);

  // Set up all pins in the mapping as OUTPUT and default LOW
  for (size_t i = 0; i < NUM_PINS; ++i) {
    pinMode(commandPinMappings[i].pin, OUTPUT);
    pinStates[i] = false; // ensure memory state LOW
    applyPinState(i);
  }
}

void loop() {
  // If still within 500 ms lockout, just resume decoder and skip processing
  if (millis() - lastCommandMillis < 500) {
    if (IrReceiver.decode()) {
      IrReceiver.resume();
    }
    return;
  }
  if (IrReceiver.decode()) {
    uint8_t cmd = IrReceiver.decodedIRData.command;
    // Iterate through mappings to find matching command
    for (size_t i = 0; i < NUM_PINS; ++i) {
      if (commandPinMappings[i].command == cmd) {
        pinStates[i] = !pinStates[i]; // toggle stored state
        applyPinState(i);
        Serial.print(F("Command 0x"));
        Serial.print(cmd, HEX);
        Serial.print(F(" set pin "));
        Serial.print(commandPinMappings[i].pin);
        Serial.print(F(" to "));
        Serial.println(pinStates[i] ? F("HIGH") : F("LOW"));
        break; // Command handled
      }
    }
    lastCommandMillis = millis(); // start lockout period
    IrReceiver.resume(); // prepare for next reception
  }
}