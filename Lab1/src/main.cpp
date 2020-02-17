#include <Arduino.h>
#define DELAY_MS 700
#define BUTTON_PIN 23

inline void turnOnLedsLightShow(volatile uint8_t& PORT, const int& msDelay);

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  DDRK = 0xFF;
  PORTK = 0;
}

void loop() {  
  if (!digitalRead(BUTTON_PIN)) {
    turnOnLedsLightShow(PORTK, DELAY_MS);
  }
}

inline void turnOnLedsLightShow(volatile uint8_t& port, const int& msDelay) {
  uint8_t start = B00000001, end = B10000000;

  while (end > start) {
    port = end;
    delay(msDelay);
    port = start;
    delay(msDelay);
    end >>= 1;
    start <<= 1;
  }
  PORTK = 0;
}
