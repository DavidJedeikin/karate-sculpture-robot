#include "bodyMotion.hpp"

namespace BodyMotion
{

void setBothArmsToAngle(Joints& joints, int angle)
{
  // setAngle method handles all out-of-bounds checking
  joints.setAngle(Joints::Name::left_shoulder, angle);
  joints.setAngle(Joints::Name::right_shoulder, angle);
}

void allJointsToZero(Joints& joints)
{
  // setAngle method handles all out-of-bounds checking
  setBothArmsToAngle(joints, 0);
  joints.setAngle(Joints::Name::waist, 0);
}

void singleDanceMotion(Joints& joints, int milliSeconds, int armOffset)
{
  // The waist joint limits are less than the arm joint limits and so we use the
  // waist joint limits as our limiting case. We sweep across the full waist
  // range and move the arms as much as possible during that motion
  Joints::Limits waistLimits = Joints::getLimits(Joints::Name::waist);
  int waistRange = Joints::getLimitsRange(Joints::Name::waist);

  uint32_t delayFor = static_cast<uint32_t>(milliSeconds / waistRange) / 2;

  // Waist goes from 0 -> MaxAngle
  for (int i = 0; i < waistLimits.maxAngle; i++)
  {
    joints.setAngle(Joints::Name::waist, i);
    joints.setAngle(Joints::Name::left_shoulder, i + armOffset);
    joints.setAngle(Joints::Name::right_shoulder, -i + armOffset);
    delay(delayFor);
  }

  // Waist goes from MaxAngle -> MinAngle
  for (int i = waistLimits.maxAngle; i > waistLimits.minAngle; i--)
  {
    joints.setAngle(Joints::Name::waist, i);
    joints.setAngle(Joints::Name::left_shoulder, i + armOffset);
    joints.setAngle(Joints::Name::right_shoulder, -i + armOffset);
    delay(delayFor);
  }

  // Waist goes from MinAngle -> 0
  for (int i = waistLimits.minAngle; i < 0; i++)
  {
    joints.setAngle(Joints::Name::waist, i);
    joints.setAngle(Joints::Name::left_shoulder, i + armOffset);
    joints.setAngle(Joints::Name::right_shoulder, -i + armOffset);
    delay(delayFor);
  }
}

} // namespace BodyMotion