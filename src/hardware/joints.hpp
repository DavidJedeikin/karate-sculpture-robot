#pragma once
#include "control/linearMap.hpp"
#include <Adafruit_PWMServoDriver.h>

#include <string>

/**
 * @brief Represents and encapsulates the three joints of the robot: Waist,
 * right shoulder, left shoulder
 *
 * Abstracts away the underlying actuation mechanism (servo motor) and only
 * exposes a method for setting the joint angle
 *
 * Underlying Library: Uses the Adafruit_PWMServoDriver library to control the
 * servo motors
 *
 */
class Joints
{
 public:
  struct Limits
  {
    int minAngle;
    int maxAngle;
  };

  enum class Name
  {
    waist,
    right_shoulder,
    left_shoulder
  };

  Joints();

  void setAngle(Name name, int angle);
  static Limits getLimits(Name name);
  static int getLimitsRange(Name name);
  static std::string toString(Name name);

 private:
  static constexpr uint32_t PULSE_SIGNAL_START{0};
  static constexpr uint32_t PWM_FREQUENCY_HZ{50};
  static constexpr uint32_t OSCILLATOR_FREQUENCY_HZ{25000000};

  // These parameters are hardware specific and define the angles required for
  // each joint to be in the kinematic zero position. The zero position is
  // defined as:
  // Waist: Torso facing forward
  // Right shoulder: Right arm inline with torso
  // Left shoulder: Left arm inline with torso
  static constexpr int WAIST_ZERO_OFFSET{85};
  static constexpr int RIGHT_SHOULDER_ZERO_OFFSET{120};
  static constexpr int LEFT_SHOULDER_ZERO_OFFSET{60};

  // Hardware specific limits for each joint
  static constexpr Limits WAIST_LIMITS{
      .minAngle = -45,
      .maxAngle = 45,
  };
  static constexpr Limits RIGHT_SHOULDER_LIMITS{
      .minAngle = -60,
      .maxAngle = 120,
  };
  static constexpr Limits LEFT_SHOULDER_LIMITS{
      .minAngle = -60,
      .maxAngle = 120,
  };

  // These parameters are for the DF-ROBOT DC5535 high torque digital servo
  // motors used on the robot
  static constexpr LinearMap::Params ANGLE_TO_DUTY_CYCLE_PARAMS{
      .inputMin = 0,    // Min angle
      .inputMax = 180,  // Max angle
      .outputMin = 60,  // Min duty cycle
      .outputMax = 450, // Max duty cycle
  };

  LinearMap angleToDutyLinearCycleMap;

  // Used to control up to 16 servo motors
  Adafruit_PWMServoDriver pwmDriverBoard;

  // Helper functions
  static uint8_t servoNumber(Name name);
  static int accountForZeroOffset(Name name, int angle);
};
