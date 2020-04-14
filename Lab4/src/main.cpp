#include <Arduino.h>

#define RIGHT_SPEED 5
#define LEFT_SPEED 6
#define LEFT_FORWARD_DIRECTION 7
#define LEFT_BACKWARD_DIRECTION 8
#define RIGHT_BACKWARD_DIRECTION 9
#define RIGHT_FORWARD_DIRECTION 11

#define MAX_SPEED 255
#define TURN_MS_TIME_DEGREE_30 150
#define TURN_MS_TIME_DEGREE_45 200
#define TURN_MS_TIME_DEGREE_60 350
#define SECOND 1000

uint8_t speedSet[2];
unsigned int timeSet[2];

uint8_t stage;

void setSpeed(const uint8_t &leftSideSpeed, const uint8_t &rightSideSpeed);
void moveForward();
void stop();
void turnLeft(const unsigned int &turnTimeMS);
void turnRight(const unsigned int &turnTimeMS);

void setup()
{
  pinMode(RIGHT_SPEED, OUTPUT);
  pinMode(LEFT_SPEED, OUTPUT);
  pinMode(LEFT_FORWARD_DIRECTION, OUTPUT);
  pinMode(RIGHT_FORWARD_DIRECTION, OUTPUT);
  pinMode(LEFT_BACKWARD_DIRECTION, OUTPUT);
  pinMode(RIGHT_BACKWARD_DIRECTION, OUTPUT);

  speedSet[0] = MAX_SPEED;
  speedSet[1] = 0.5 * MAX_SPEED;
  timeSet[0] = 5 * SECOND;
  timeSet[1] = 4 * SECOND;
}

void loop()
{
  for (stage = 1; stage <= 4; stage++) {
    if (stage % 2) {
      setSpeed(speedSet[0], speedSet[0]);
      moveForward();
      delay(timeSet[0]);
      stop();
    }
    else {
      setSpeed(speedSet[1], speedSet[1]);
      moveForward();
      delay(timeSet[1]);
      stop();
    }

    if (stage == 1 || stage == 4) {
      turnLeft(TURN_MS_TIME_DEGREE_60);
    } else {
      turnRight(TURN_MS_TIME_DEGREE_60);
    }
  }
}

void setSpeed(const uint8_t &leftSideSpeed, const uint8_t &rightSideSpeed)
{
  analogWrite(LEFT_SPEED, leftSideSpeed);
  analogWrite(RIGHT_SPEED, rightSideSpeed);
}

void moveForward()
{
  digitalWrite(LEFT_FORWARD_DIRECTION, HIGH);
  digitalWrite(RIGHT_FORWARD_DIRECTION, HIGH);
  digitalWrite(LEFT_BACKWARD_DIRECTION, LOW);
  digitalWrite(RIGHT_BACKWARD_DIRECTION, LOW);
}

void stop()
{
  digitalWrite(LEFT_FORWARD_DIRECTION, LOW);
  digitalWrite(RIGHT_FORWARD_DIRECTION, LOW);
  digitalWrite(LEFT_BACKWARD_DIRECTION, LOW);
  digitalWrite(RIGHT_BACKWARD_DIRECTION, LOW);
  setSpeed(0, 0);
}

void turnLeft(const unsigned int &turnTimeMS)
{
  setSpeed(MAX_SPEED, MAX_SPEED);
  digitalWrite(LEFT_FORWARD_DIRECTION, LOW);
  digitalWrite(RIGHT_FORWARD_DIRECTION, HIGH);
  digitalWrite(LEFT_BACKWARD_DIRECTION, HIGH);
  digitalWrite(RIGHT_BACKWARD_DIRECTION, LOW);

  unsigned int startedAt = millis();
  while (true)
  {
    if (millis() - startedAt > turnTimeMS)
    {
      stop();
      break;
    }
  }
}

void turnRight(const unsigned int &turnTimeMS)
{
  setSpeed(MAX_SPEED, MAX_SPEED);
  digitalWrite(LEFT_FORWARD_DIRECTION, HIGH);
  digitalWrite(RIGHT_FORWARD_DIRECTION, LOW);
  digitalWrite(LEFT_BACKWARD_DIRECTION, LOW);
  digitalWrite(RIGHT_BACKWARD_DIRECTION, HIGH);

  unsigned int startedAt = millis();
  while (true)
  {
    if (millis() - startedAt > turnTimeMS)
    {
      stop();
      break;
    }
  }
}
