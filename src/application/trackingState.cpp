#include "trackingState.hpp"
#include "logging/log.hpp"
#include "motion/bodyMotion.hpp"
#include <algorithm>
#include <utility>

TrackingState::TrackingState(std::shared_ptr<Hardware> hardware)
    : hardware(hardware), pidController(this->pidParams),
      waistLimits(Joints::getLimits(Joints::Name::waist)), tooCloseState(*this),
      withinRangeState(*this), outOfRangeState(*this)
{
  // For safety reasons, we assume an object is right in front of the robot at
  // start up
  this->tooCloseState.enter();
}

void TrackingState::enter()
{
  LOG_INFO("Entering the %s", this->name());
  this->hardware->eyes.setColour(Eyes::Colour::green);
  BodyMotion::setBothArmsToAngle(this->hardware->joints, ARM_ANGLE);
  this->setWaistAngle(0);
}

void TrackingState::runOnce()
{
  auto* desiredState = this->getDesiredState();
  if (desiredState == nullptr)
  {
    LOG_WARN("%s", "Desired state is a nullptr");
    return;
  }
  if (this->currentState != desiredState)
  {
    desiredState->enter();
  }
  else
  {
    this->currentState->runOnce();
  }
}

char const* TrackingState::name()
{
  return "TrackingState";
}

void TrackingState::setWaistAngle(int angle)
{
  if (angle < this->waistLimits.minAngle || angle > this->waistLimits.maxAngle)
  {
    LOG_WARN("Attempted to set a waist angle: %d out of bounds: [%d, %d] - "
             "clamping at: %d",
             angle,
             this->waistLimits.minAngle,
             this->waistLimits.maxAngle,
             angle < this->waistLimits.minAngle ? this->waistLimits.minAngle
                                                : this->waistLimits.maxAngle);
    this->waistAngle = std::clamp(
        angle, this->waistLimits.minAngle, this->waistLimits.maxAngle);
  }
  this->hardware->joints.setAngle(Joints::Name::waist, this->waistAngle);
}

//////////////////////////////////////////////////////////////////////
// Desired State Selector
//////////////////////////////////////////////////////////////////////

IState* TrackingState::getDesiredState()
{
  this->objectDistance = this->hardware->sonarArray.getDistance().min;
  if (this->objectDistance < MIN_DISTANCE_CM)
  {
    return &this->tooCloseState;
  }
  if (this->objectDistance > MAX_DISTANCE_CM)
  {
    return &this->outOfRangeState;
  }
  return &this->withinRangeState;
}

//////////////////////////////////////////////////////////////////////
// Base State
//////////////////////////////////////////////////////////////////////

TrackingState::State::State(TrackingState& parent,
                            std::string&& stateName,
                            Eyes::Colour eyeColour,
                            int eyeTransitionTime)
    : parent(parent), stateName(std::move(stateName)), eyeColour(eyeColour),
      eyeTransitionTime(eyeTransitionTime)
{
}

void TrackingState::State::enter()
{
  LOG_INFO("Entering %s::%s", this->parent.name(), this->name());
  this->parent.waistAngle = 0;
  this->parent.hardware->joints.setAngle(Joints::Name::waist,
                                         this->parent.waistAngle);
  this->parent.currentState = this;
  this->parent.hardware->eyes.crossFade(
      this->parent.currentEyeColour, this->eyeColour, this->eyeTransitionTime);
  this->parent.currentEyeColour = this->eyeColour;
}

char const* TrackingState::State::name()
{
  return this->stateName.c_str();
}

//////////////////////////////////////////////////////////////////////
// TooCloseState
//////////////////////////////////////////////////////////////////////

TrackingState::TooCloseState::TooCloseState(TrackingState& parent)
    : State(parent, "TooCloseState", Eyes::Colour::off, EYE_TRANSITION_TIME)
{
}

void TrackingState::TooCloseState::enter()
{
  State::enter();
}

void TrackingState::TooCloseState::runOnce()
{
  LOG_INFO("Running once %s::%s", this->parent.name(), this->name());
}

//////////////////////////////////////////////////////////////////////
// WithinRangeState
//////////////////////////////////////////////////////////////////////

TrackingState::WithinRangeState::WithinRangeState(TrackingState& parent)
    : State(parent, "WithinRangeState", Eyes::Colour::red, EYE_TRANSITION_TIME)
{
}

void TrackingState::WithinRangeState::enter()
{
  State::enter();
}

// Here we apply the very simple control algorithm which is used to a track an
// object in front of the robot
void TrackingState::WithinRangeState::runOnce()
{
  SonarArray::Distance distance =
      this->parent.hardware->sonarArray.getDistance();

  float difference = distance.right - distance.left;

  int controlSignal = static_cast<int>(
      this->parent.pidController.getControlSignal(difference, 0));

  if (std::fabs(difference) > 5)
  {
    this->parent.waistAngle += controlSignal;
  }

  this->parent.setWaistAngle(this->parent.waistAngle);

  delay(this->parent.pidParams.timestepMs);
}

//////////////////////////////////////////////////////////////////////
// OutOfRangeState
//////////////////////////////////////////////////////////////////////

TrackingState::OutOfRangeState::OutOfRangeState(TrackingState& parent)
    : State(parent,
            "OutOfRangeState",
            Eyes::Colour::light_blue,
            EYE_TRANSITION_TIME)
{
}

void TrackingState::OutOfRangeState::enter()
{
  State::enter();
}

void TrackingState::OutOfRangeState::runOnce()
{
  LOG_INFO("Running once %s::%s", this->parent.name(), this->name());
}