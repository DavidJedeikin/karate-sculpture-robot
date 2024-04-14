#pragma once

/**
 * @brief Representation of a State in the State Machine
 *
 * A state can either be entered (enter method), and an execution cycle can take
 * place within a state (runOnce method). A State must have a name (name method)
 *
 * The "I" preface is used to indicate that the state is an Interface class and
 * is fully abstract.
 *
 */
class IState
{
 public:
  virtual ~IState() = default;
  virtual void enter() = 0;
  virtual void runOnce() = 0;
  virtual char const* name() = 0;
};
