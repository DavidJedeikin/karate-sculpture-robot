#pragma once
#include "hardware/hardware.hpp"
#include "iState.hpp"
#include <memory>

/**
 * @brief High-level Application class that manages the high-level StateMachine
 * (DanceState and TrackingState)
 *
 * The desired state is determined in the getDesiredState method. If the desired
 * state is new, the new state is entered, otherwise the state is runOnce
 *
 */
class Application
{
 public:
  Application();
  void run();

 private:
  std::shared_ptr<Hardware> hardware{nullptr};

  // State Machine
  std::unique_ptr<IState> danceState{nullptr};
  std::unique_ptr<IState> trackingState{nullptr};

  IState* getDesiredState();
  IState* currentState{nullptr};
};