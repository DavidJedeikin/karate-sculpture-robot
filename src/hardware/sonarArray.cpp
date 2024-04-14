#include "sonarArray.hpp"
#include "Arduino.h"
#include "logging/log.hpp"

SonarArray::SonarArray()
{
  // Set up the pins
  pinMode(this->rightSensor.triggerPin, OUTPUT);
  pinMode(this->rightSensor.echoPin, INPUT);
  pinMode(this->leftSensor.triggerPin, OUTPUT);
  pinMode(this->leftSensor.echoPin, INPUT);
}

SonarArray::Distance SonarArray::getDistance()
{

  float distanceFromRightSensor = getDistanceFromSensor(this->rightSensor);

  delay(30); // Delay to prevent interference between the two sensors

  float distanceFromLeftSensor = getDistanceFromSensor(this->leftSensor);

  float min = std::min(distanceFromRightSensor, distanceFromLeftSensor);

  return {.right = distanceFromRightSensor,
          .left = distanceFromLeftSensor,
          .min = min};
}

float SonarArray::getDistanceFromSensor(Hcsr04SensorPins sensor)
{
  // This article explains what is going on here better than I can in a few
  // comments - give is a squiz if you're interested in the details:
  // https://www.electronicwings.com/nodemcu/hc-sr04-ultrasonic-sensor-interfacing-with-nodemcu

  // Pulse the trigger pin to generate a sound wave
  digitalWrite(sensor.triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(sensor.triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(sensor.triggerPin, LOW);

  // Essentially we are measuring the time it takes for sound wave to hit an
  // object and bounce back
  auto timeForWaveToBounceAndReflectUs =
      static_cast<float>(pulseIn(sensor.echoPin, HIGH));

  float timeForWaveToTravelOneWayS =
      timeForWaveToBounceAndReflectUs / 2 * MICROSECONDS_TO_SECONDS;

  // Distance = time * speed of sound
  float distanceToObjectCm = timeForWaveToTravelOneWayS * SPEED_OF_SOUND;

  return distanceToObjectCm;
}

std::string SonarArray::Distance::toString() const
{
  return format(
      "Right: %.0f, Left: %.0f Min: %.0f", this->right, this->left, this->min);
}
