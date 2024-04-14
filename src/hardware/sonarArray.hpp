#pragma once
#include <string>

/**
 * @brief Represents and encapsulates two HC-SR04 two sonar sensors which
 * together form the Sonar Array. The two sensors are mounted at the front of
 * the robot, at an angle of 15 degrees apart. Each sensors measures the
 * distance to the nearest object in front of it. These two distances can be
 * combined to determine whether the closest object is to the left, right or
 * straight ahead.
 *
 * Abstracts away the underlying GPIO ineraction
 *
 * Underlying Library: Uses the digital_wiring library to read the state of the
 * two HC-SR04 ultrasonic sensors.
 *
 */
class SonarArray
{
 public:
  struct Distance
  {
    float right;
    float left;
    float min;
    [[nodiscard]] std::string toString() const;
  };

  SonarArray();
  Distance getDistance();

 private:
  struct Hcsr04SensorPins
  {
    int triggerPin;
    int echoPin;
  };

  // Hardware specific pin configuration
  Hcsr04SensorPins rightSensor{.triggerPin = 7, .echoPin = 11};
  Hcsr04SensorPins leftSensor{.triggerPin = 16, .echoPin = 15};

  // Physics constants
  static constexpr float METERS_TO_CM{100.0F};
  static constexpr float SPEED_OF_SOUND{343.0F};
  static constexpr float MICROSECONDS_TO_SECONDS{1 / 1000000.0F};

  static float getDistanceFromSensor(Hcsr04SensorPins sensor);
};