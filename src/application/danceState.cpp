#include "danceState.hpp"
#include "logging/log.hpp"
#include "motion/bodyMotion.hpp"
#include <memory>

DanceState::DanceState(std::shared_ptr<Hardware> hardware)
    : hardware(hardware), distanceToSpeed(distanceToSpeedParams),
      tooCloseState(*this), withinRangeState(*this), outOfRangeState(*this)
{
  // For safety reasons, we assume an object is right in front of the robot at
  // start up
  this->tooCloseState.enter();
}

void DanceState::enter()
{
  LOG_INFO("Entering the %s", this->name());

  // Having a bit of fun with different eye colours
  for (int i = 0; i < 5; i++)
  {
    this->hardware->eyes.setColour(Eyes::Colour::red);
    delay(100);
    this->hardware->eyes.setColour(Eyes::Colour::light_blue);
    delay(100);
  }
  this->currentEyeColour = Eyes::Colour::light_blue;
  delay(100);
}

void DanceState::runOnce()
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

char const* DanceState::name()
{
  return "DanceState";
}

//////////////////////////////////////////////////////////////////////
// Desired State Selector
//////////////////////////////////////////////////////////////////////

IState* DanceState::getDesiredState()
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

DanceState::State::State(DanceState& parent,
                         std::string&& stateName,
                         Eyes::Colour eyeColour,
                         int eyeTransitionTime)
    : parent(parent), stateName(std::move(stateName)), eyeColour(eyeColour),
      eyeTransitionTime(eyeTransitionTime)
{
}

void DanceState::State::enter()
{
  LOG_INFO("Entering %s::%s", this->parent.name(), this->name());
  this->parent.currentState = this;
  this->parent.hardware->eyes.crossFade(
      this->parent.currentEyeColour, this->eyeColour, this->eyeTransitionTime);
  this->parent.currentEyeColour = this->eyeColour;
}

char const* DanceState::State::name()
{
  return this->stateName.c_str();
}

//////////////////////////////////////////////////////////////////////
// TooCloseState
//////////////////////////////////////////////////////////////////////

DanceState::TooCloseState::TooCloseState(DanceState& parent)
    : State(parent, "TooCloseState", Eyes::Colour::off, EYE_TRANSITION_TIME)
{
}

void DanceState::TooCloseState::enter()
{
  BodyMotion::setBothArmsToAngle(this->parent.hardware->joints, -30);
  State::enter();
}

void DanceState::TooCloseState::runOnce()
{
  LOG_INFO("Running %s::%s", this->parent.name(), this->name());
}

//////////////////////////////////////////////////////////////////////
// WithinRangeState
//////////////////////////////////////////////////////////////////////

DanceState::WithinRangeState::WithinRangeState(DanceState& parent)
    : State(parent, "WithinRangeState", Eyes::Colour::red, EYE_TRANSITION_TIME)
{
}

void DanceState::WithinRangeState::enter()
{
  State::enter();

  // Have more fun with changing the eye colour
  for (int i = 0; i < 3; i++)
  {
    this->parent.hardware->eyes.crossFade(
        Eyes::Colour::blue, Eyes::Colour::red, 100);
    this->parent.hardware->eyes.crossFade(
        Eyes::Colour::red, Eyes::Colour::red, 100);
  }
}

void DanceState::WithinRangeState::runOnce()
{
  LOG_INFO("Running %s::%s", this->parent.name(), this->name());
  this->danceSpeed = static_cast<int>(
      this->parent.distanceToSpeed.getOutput(this->parent.objectDistance));
  BodyMotion::singleDanceMotion(this->parent.hardware->joints,
                                this->danceSpeed,
                                this->parent.armMotionOffset);
}

//////////////////////////////////////////////////////////////////////
// OutOfRangeState
//////////////////////////////////////////////////////////////////////

DanceState::OutOfRangeState::OutOfRangeState(DanceState& parent)
    : State(parent,
            "OutOfRangeState",
            Eyes::Colour::light_blue,
            EYE_TRANSITION_TIME)
{
}

void DanceState::OutOfRangeState::enter()
{
  State::enter();
}

void DanceState::OutOfRangeState::runOnce()
{
  LOG_INFO("Running %s::%s", this->parent.name(), this->name());
}
