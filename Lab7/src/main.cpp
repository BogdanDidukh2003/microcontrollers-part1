#include <Arduino.h>

#define MEMORY_REGISTER_LENGTH 10
#define BUZZER 44  // PD1
#define BUTTON1 36 // PC1
#define BUTTON2 35 // PC2
#define BUTTON3 34 // PC3
#define BUTTON4 33 // PC4
#define BEEP_SHORT_DELAY 30
#define BEEP_LONG_DELAY 100
#define DOT 0b01111111

uint8_t segmentNumber[]{
    0b11000000,
    0b11111001,
    0b10100100,
    0b10110000,
    0b10011001,
    0b10010010,
    0b10000010,
    0b11111000,
    0b10000000,
    0b10010000};

struct Time
{
  uint8_t hour, minute, second;
};

Time time = {0, 0, 0};
Time memoryRegister[MEMORY_REGISTER_LENGTH];

bool isBeep = false, isStopped = false, showMemory = false;
uint8_t selectedKey, memoryRegisterSize = 0;
int8_t memoryIndexToShow = -1;

void setNumberOnDisplaySegment(const uint8_t &indexOfSegment = 255, const uint8_t &number = 0, const bool &dot = false);
inline void showTimeOnSegment(const uint8_t &indexOfSegment, const Time &time);
inline void onPressKey1();
inline void onPressKey2();
inline void onPressKey3();
inline void onPressKey4();
inline void resetMemoryRegister();
inline bool saveTimeToMemoryRegister();
void shortBeep();
void longBeep();

ISR(TIMER2_OVF_vect)
{
  if (!isStopped)
  {
    if (++time.second == 60)
    {
      time.second = 0;
      if (++time.minute == 60)
      {
        time.minute = 0;
        if (++time.hour == 24)
          time.hour = 0;
      }
    }

    if (time.second == 0)
    {
      isBeep = true;
    }
  }
}

uint8_t displayIteration = 0;
ISR(TIMER5_COMPA_vect)
{
  if (showMemory)
  {
    showTimeOnSegment(displayIteration, memoryRegister[memoryIndexToShow]);
  }
  else
  {
    showTimeOnSegment(displayIteration, time);
  }

  displayIteration++;
  if (displayIteration > 5)
  {
    displayIteration = 0;
  }
}

void setup()
{
  noInterrupts();
  DDRD = 0xFF;
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);
  pinMode(BUTTON4, INPUT_PULLUP);

  DDRB = 0xFF; // for controling segments
  PORTB = 0xFF;
  DDRA = 0xFF; // powering segments
  PORTA = 0xFF;

  ASSR = (1 << AS2);
  TCCR2B = (1 << CS22);
  while (ASSR & ((1 << TCN2UB) | (1 << OCR2BUB) | (1 << TCR2BUB)))
  {
  }
  TIFR2 = 0x00;
  TIMSK2 = (1 << TOIE2);

  TCCR5A = WGM50;
  TCCR5B = (1 << WGM52) | (1 << CS52) | (1 << CS50);
  TIMSK5 = (1 << OCIE5A);
  OCR5A = 0x07;

  interrupts();
}

void loop()
{
  if (!digitalRead(BUTTON1))
  {
    selectedKey = 1;
  }
  else if (!digitalRead(BUTTON2))
  {
    selectedKey = 2;
  }
  else if (!digitalRead(BUTTON3))
  {
    selectedKey = 3;
  }
  else if (!digitalRead(BUTTON4))
  {
    selectedKey = 4;
  }
  else
  {
    selectedKey = 0;
  }

  switch (selectedKey)
  {
  case 1:
    onPressKey1();
    break;

  case 2:
    onPressKey2();
    break;

  case 3:
    onPressKey3();
    break;

  case 4:
    onPressKey4();
    break;

  default:
    break;
  }

  if (isBeep)
  {
    longBeep();
    isBeep = false;
  }
}

inline void onPressKey1()
{
  if (isStopped)
  {
    time = {0, 0, 0};
    showMemory = false;
    memoryIndexToShow = -1;
  }
  isStopped = !isStopped;

  shortBeep();
}

inline void onPressKey2()
{
  if (saveTimeToMemoryRegister())
  {
    shortBeep();
  }
  else
  {
    longBeep();
  }
}

inline void onPressKey3()
{
  resetMemoryRegister();
  shortBeep();
}

inline void onPressKey4()
{
  shortBeep();
  if (isStopped)
  {
    if (memoryIndexToShow < 0 && !memoryRegisterSize)
    {
      return;
    }

    memoryIndexToShow++;
    if (memoryIndexToShow >= memoryRegisterSize)
    {
      memoryIndexToShow = 0;
    }
    showMemory = true;
  }
}

inline void resetMemoryRegister()
{
  memoryRegisterSize = 0;
}

inline bool saveTimeToMemoryRegister()
{
  if (memoryRegisterSize < MEMORY_REGISTER_LENGTH)
  {
    memoryRegister[memoryRegisterSize++] = time;
    return true;
  }
  return false;
}

void setNumberOnDisplaySegment(const uint8_t &indexOfSegment = 255, const uint8_t &number = 0, const bool &dot = false)
{
  if (indexOfSegment < 6 && indexOfSegment >= 0 && number < 10 && number >= 0)
  {
    PORTA = ~(1 << indexOfSegment);
    PORTB = segmentNumber[number];
    if (dot)
    {
      PORTB &= DOT;
    }
  }
}

inline void showTimeOnSegment(const uint8_t &indexOfSegment, const Time &time)
{
  switch (indexOfSegment)
  {
  case 0:
    setNumberOnDisplaySegment(0, int(time.hour / 10));
    break;

  case 1:
    setNumberOnDisplaySegment(1, time.hour - int(time.hour / 10) * 10, true);
    break;

  case 2:
    setNumberOnDisplaySegment(2, int(time.minute / 10));
    break;

  case 3:
    setNumberOnDisplaySegment(3, time.minute - int(time.minute / 10) * 10, true);
    break;

  case 4:
    setNumberOnDisplaySegment(4, int(time.second / 10));
    break;

  case 5:
    setNumberOnDisplaySegment(5, time.second - int(time.second / 10) * 10);
    break;

  default:
    break;
  }
}

void shortBeep()
{
  digitalWrite(BUZZER, HIGH);
  delay(BEEP_SHORT_DELAY);
  digitalWrite(BUZZER, LOW);
}

void longBeep()
{
  digitalWrite(BUZZER, HIGH);
  delay(BEEP_LONG_DELAY);
  digitalWrite(BUZZER, LOW);
}
