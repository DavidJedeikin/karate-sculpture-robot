#include "switch.hpp"

Switch::Switch()
{
  pinMode(this->switchPin, INPUT_PULLDOWN);
}

Switch::State Switch::getState() const
{
  return digitalRead(this->switchPin) == 0 ? State::off : State::on;
}

std::string Switch::toString(State state)
{
  switch (state)
  {
    case State::on:
      return "ON";
    case State::off:
      return "OFF";
  }
}
