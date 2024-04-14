#pragma once
#include "Arduino.h"
#include <string>

/**
 * @brief Represents and encapsulates the physical state switch of the robot.
 *
 * Abstracts away the underlying GPIO ineraction
 *
 * Underlying Library: Uses the digital_wiring library to read the state of the
 * switch
 *
 */

class Switch
{
 public:
  enum class State
  {
    on,
    off
  };

  Switch();

  [[nodiscard]] State getState() const;
  static std::string toString(State state);

 private:
  int switchPin{A3};
};
