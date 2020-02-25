#include <Arduino.h>

#define DELAY_MS 700
#define BUTTON1 23
#define BUTTON2 25
#define LEDS_NUMBER 8

const int ledPins[] = {A8, A9, A10, A11, A12, A13, A14, A15};

inline void turnOnLedsLightShowAlgortihm1(const int& msDelay);
inline void turnOnLedsLightShowAlgortihm2(const int& msDelay);
inline void sendAlgorithm1ToSerial(const int& msDelay);
inline void sendAlgorithm2ToSerial(const int& msDelay);

void setup() {
  for (auto& ledPin : ledPins) {
    pinMode(ledPin, OUTPUT);
  }
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    switch (Serial.read()) {
      
    case '1':
      turnOnLedsLightShowAlgortihm1(DELAY_MS);
      break;

    case '2':
      turnOnLedsLightShowAlgortihm2(DELAY_MS);
      break;
    }
  }
  if (!digitalRead(BUTTON1)) {
    sendAlgorithm1ToSerial(DELAY_MS);
    delay(50);
  }
  if (!digitalRead(BUTTON2)) {
    sendAlgorithm2ToSerial(DELAY_MS);
    delay(50);
  }
}

inline void turnOnLedsLightShowAlgortihm1(const int& msDelay) {
  for (int led = 0; led < LEDS_NUMBER / 2; led++) {
    digitalWrite(ledPins[LEDS_NUMBER - 1 - led], HIGH);
    delay(msDelay);
    digitalWrite(ledPins[LEDS_NUMBER - 1 - led], LOW);
    
    digitalWrite(ledPins[led], HIGH);
    delay(msDelay);
    digitalWrite(ledPins[led], LOW);
  }
}

inline void turnOnLedsLightShowAlgortihm2(const int& msDelay) {
  for (int led = 0; led < LEDS_NUMBER / 2; led++) {
    digitalWrite(ledPins[led], HIGH);
    digitalWrite(ledPins[LEDS_NUMBER - 1 - led], HIGH);
    delay(msDelay);
    digitalWrite(ledPins[led], LOW);
    digitalWrite(ledPins[LEDS_NUMBER - 1 - led], LOW);
  }
}

inline void sendAlgorithm1ToSerial(const int& msDelay) {
  String data = "";
  for (int led = 0; led < LEDS_NUMBER / 2; led++) {
    data += String(ledPins[LEDS_NUMBER - 1 - led]) + ",";
    data += String(ledPins[led]) + ",";
  }
  data += String(DELAY_MS) + ",";
  data += String(1);
  Serial.println(data);
}

inline void sendAlgorithm2ToSerial(const int& msDelay) {
  String data = "";
  for (int led = 0; led < LEDS_NUMBER / 2; led++) {
    data += String(ledPins[led]) + ",";
    data += String(ledPins[LEDS_NUMBER - 1 - led]) + ",";
  }
  data += String(DELAY_MS) + ",";
  data += String(2);
  Serial.println(data);
}
