#pragma once
#include "control/linearMap.hpp"
#include "hardware/hardware.hpp"
#include "iState.hpp"
#include <memory>

class DanceState : public IState
{
 public:
  DanceState(std::shared_ptr<Hardware> hardware);
  void enter() override;
  void runOnce() override;
  char const* name() override;

 private:
  std::shared_ptr<Hardware> hardware;

  float objectDistance{0};
  Eyes::Colour currentEyeColour{Eyes::Colour::light_blue};
  static constexpr uint32_t EYE_TRANSITION_TIME{500};

  //////////////////////////////////////////////////////////////////////
  // Motion Params
  //////////////////////////////////////////////////////////////////////
  int armMotionOffset{-15};

  //////////////////////////////////////////////////////////////////////
  // Distance Params
  //////////////////////////////////////////////////////////////////////
  static constexpr float MIN_DISTANCE_CM{25};
  static constexpr float MAX_DISTANCE_CM{75};
  static constexpr LinearMap::Params distanceToSpeedParams{
      .inputMin = MIN_DISTANCE_CM,
      .inputMax = MAX_DISTANCE_CM,
      .outputMin = 500,
      .outputMax = 3000};
  LinearMap distanceToSpeed;

  //////////////////////////////////////////////////////////////////////
  // DanceState Internal State Machine
  //////////////////////////////////////////////////////////////////////

  IState* currentState{nullptr};
  IState* getDesiredState();

  /**
   * @brief Extend IState to State enabling DanceState-specific internal
   * State representation
   *
   */
  class State : public IState
  {
   public:
    State(DanceState& parent,
          std::string&& stateName,
          Eyes::Colour eyeColour,
          int eyeTransitionTime);
    void enter() override;
    char const* name() override;

   protected:
    DanceState& parent;
    std::string stateName;
    Eyes::Colour eyeColour;
    int eyeTransitionTime;
  };

  class TooCloseState : public State

  {
   public:
    TooCloseState(DanceState& parent);
    void enter() override;
    void runOnce() override;
  } tooCloseState;

  class WithinRangeState : public State
  {
   public:
    WithinRangeState(DanceState& parent);
    void enter() override;
    void runOnce() override;

   private:
    int danceSpeed{
        static_cast<int>(DanceState::distanceToSpeedParams.outputMax)};
  } withinRangeState;

  class OutOfRangeState : public State
  {
   public:
    OutOfRangeState(DanceState& parent);
    void enter() override;
    void runOnce() override;

  } outOfRangeState;
};