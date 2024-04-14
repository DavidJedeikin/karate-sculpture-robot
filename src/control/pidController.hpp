#pragma once

#include <cstdint>
#include <string>

/**
 * @brief PID controller class
 *
 */
class PidController
{
 public:
  struct Parameters
  {
    float Kp{0.0F};
    float Kd{0.0F};
    float Ki{0.0F};
    uint32_t timestepMs{0};
    float maxControlSignal{0.0F};
    float minControlSignal{0.0F};
    float windupLimitFactor{0.8F};
    [[nodiscard]] std::string toString() const;
  };

  PidController(Parameters params);

  void updateKp(float Kp);
  void updateKd(float Kd);
  void updateKi(float Ki);

  float getControlSignal(float currentState, float targetState);

 private:
  Parameters params;

  float timestepSeconds{0.0F};
  float errorIntegral{0.0F};
  float previousError{0.0F};

  void applyAntiIntegralWindupMechanism();
};
