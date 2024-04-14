#pragma once
#include "control/pidController.hpp"
#include "hardware/hardware.hpp"
#include "iState.hpp"
#include <memory>

/**
 * @brief In this state the robot attempts to "track" (or face) the object in
 * front of it. It does this by rotating left or right depending on
 * whether the object is more to the left or more to the right of the robot.
 *
 */
class TrackingState : public IState
{
 public:
  TrackingState(std::shared_ptr<Hardware> hardware);
  void enter() override;
  void runOnce() override;
  char const* name() override;

 private:
  std::shared_ptr<Hardware> hardware;

  int waistAngle{0};
  float objectDistance{0};
  Eyes::Colour currentEyeColour{Eyes::Colour::light_blue};

  static constexpr uint32_t EYE_TRANSITION_TIME{500};
  static constexpr float ARM_ANGLE{-50};
  static constexpr float MIN_DISTANCE_CM{15};
  static constexpr float MAX_DISTANCE_CM{85};

  void setWaistAngle(int angle);

  //////////////////////////////////////////////////////////////////////
  // Control
  //////////////////////////////////////////////////////////////////////

  static constexpr float MAX_ANGLE_CHANGE{10};
  PidController::Parameters pidParams{.Kp = 1.5,
                                      .Kd = 0.0,
                                      .Ki = 0,
                                      .timestepMs = 50,
                                      .maxControlSignal = MAX_ANGLE_CHANGE,
                                      .minControlSignal = -MAX_ANGLE_CHANGE};

  PidController pidController;
  Joints::Limits waistLimits;

  //////////////////////////////////////////////////////////////////////
  // TrackingState internal StateMachine
  //////////////////////////////////////////////////////////////////////

  IState* currentState{nullptr};

  IState* getDesiredState();

  /**
   * @brief Extend IState to State enabling TrackingState specific internal
   * State representation
   *
   */
  class State : public IState
  {
   public:
    State(TrackingState& parent,
          std::string&& stateName,
          Eyes::Colour eyeColour,
          int eyeTransitionTime);
    void enter() override;
    char const* name() override;

   protected:
    TrackingState& parent;
    std::string stateName;
    Eyes::Colour eyeColour;
    int eyeTransitionTime;
  };

  class TooCloseState : public State
  {
   public:
    TooCloseState(TrackingState& parent);
    void enter() override;
    void runOnce() override;
  } tooCloseState;

  class WithinRangeState : public State
  {
   public:
    WithinRangeState(TrackingState& parent);
    void enter() override;
    void runOnce() override;
  } withinRangeState;

  class OutOfRangeState : public State
  {
   public:
    OutOfRangeState(TrackingState& parent);
    void enter() override;
    void runOnce() override;

  } outOfRangeState;
};