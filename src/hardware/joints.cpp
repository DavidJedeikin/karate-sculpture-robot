#include "joints.hpp"
#include "logging/log.hpp"
#include <algorithm>

Joints::Joints() : angleToDutyLinearCycleMap(ANGLE_TO_DUTY_CYCLE_PARAMS)
{
  // Configure servo driver board
  this->pwmDriverBoard.begin();
  this->pwmDriverBoard.setPWMFreq(PWM_FREQUENCY_HZ);
  this->pwmDriverBoard.setOscillatorFrequency(OSCILLATOR_FREQUENCY_HZ);
  delay(10);

  // Set all joints to the zero position
  this->setAngle(Name::waist, 0);
  this->setAngle(Name::left_shoulder, 0);
  this->setAngle(Name::right_shoulder, 0);

  LOG_INFO("Setting %s, %s & %s to position: 0",
           this->toString(Name::waist),
           this->toString(Name::left_shoulder),
           this->toString(Name::right_shoulder));
}

void Joints::setAngle(Name name, int angle)
{
  Limits limits = Joints::getLimits(name);

  // Check if the angle is within the allowable hardware limits. If not, clamp
  // the angle and log a warning - don't fail silently
  if (angle < limits.minAngle || angle > limits.maxAngle)
  {
    int angleRequest = angle;
    angle = std::clamp(angleRequest, limits.minAngle, limits.maxAngle);

    LOG_WARN("Attempted to set an angle: %d, which is out of bound [%d, %d] - "
             "Clamping angle at: %d",
             angleRequest,
             limits.minAngle,
             limits.maxAngle,
             angle);
  }

  int offsetAngle = Joints::accountForZeroOffset(name, angle);
  uint32_t servoNumber = Joints::servoNumber(name);
  auto dutyCycle =
      static_cast<uint32_t>(this->angleToDutyLinearCycleMap.getOutput(
          static_cast<float>(offsetAngle)));
  this->pwmDriverBoard.setPWM(servoNumber, PULSE_SIGNAL_START, dutyCycle);
}

int Joints::accountForZeroOffset(Name name, int angle)
{
  switch (name)
  {
    case Name::waist:
      return WAIST_ZERO_OFFSET + angle;
    case Name::left_shoulder:
      return LEFT_SHOULDER_ZERO_OFFSET + angle;
    case Name::right_shoulder:
      return RIGHT_SHOULDER_ZERO_OFFSET - angle;
  }
}

Joints::Limits Joints::getLimits(Name name)
{
  switch (name)
  {
    case Name::waist:
      return WAIST_LIMITS;
    case Name::right_shoulder:
      return RIGHT_SHOULDER_LIMITS;
    case Name::left_shoulder:
      return LEFT_SHOULDER_LIMITS;
  }
}

int Joints::getLimitsRange(Name name)
{
  auto limits = Joints::getLimits(name);
  return limits.maxAngle - limits.minAngle;
}

uint8_t Joints::servoNumber(Name name)
{
  switch (name)
  {
    case Name::left_shoulder:
      return 0;
    case Name::right_shoulder:
      return 1;
    case Name::waist:
      return 2;
  }
}

std::string Joints::toString(Name name)
{
  switch (name)
  {
    case Name::waist:
      return "WAIST";
    case Name::right_shoulder:
      return "RIGHT_SHOULDER";
    case Name::left_shoulder:
      return "LEFT_SHOULDER";
  }
}
