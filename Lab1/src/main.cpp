#include <Arduino.h>
#define DELAY_MS 700

const int ledPins[] = {A8, A9, A10, A11, A12, A13, A14, A15};
const int buttonPin = 23;
bool buttonIsPressed = false;

const int ledPinsSize = sizeof(ledPins) / sizeof(ledPins[0]);

inline void turnOnLedsLightShow(const int& msDelay);

void setup() {
  for (auto& ledPin : ledPins) {
    pinMode(ledPin, OUTPUT);
  }
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  buttonIsPressed = !digitalRead(buttonPin);
  
  if (buttonIsPressed) {
    turnOnLedsLightShow(DELAY_MS);
  }
}

inline void turnOnLedsLightShow(const int& msDelay) {
  for (int led = 0; led < ledPinsSize / 2; led++) {
    digitalWrite(ledPins[ledPinsSize - 1 - led], HIGH);
    delay(msDelay);
    digitalWrite(ledPins[ledPinsSize - 1 - led], LOW);
    
    digitalWrite(ledPins[led], HIGH);
    delay(msDelay);
    digitalWrite(ledPins[led], LOW);
  }
}
