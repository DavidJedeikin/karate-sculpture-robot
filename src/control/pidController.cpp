
#include "pidController.hpp"
#include "logging/log.hpp"
#include <algorithm>

PidController::PidController(Parameters params)
    : params(params),
      timestepSeconds(static_cast<float>(params.timestepMs) / 1000.0F)
{
}

void PidController::updateKp(float Kp)
{
  this->params.Kp = Kp;
}

void PidController::updateKd(float Kd)
{
  this->params.Kd = Kd;
}

void PidController::updateKi(float Ki)
{
  // Adjust the integral term if necessary
  if (Ki > this->params.Ki && this->errorIntegral != 0.0)
  {
    // Adjust the integral term so the next control effort is unaffected and we
    // don't see a jump in the output
    float increaseRatio = Ki / this->params.Ki;
    this->errorIntegral /= increaseRatio;
  }
  else if (Ki == 0.0)
  {
    this->errorIntegral = 0.0;
  }

  this->params.Ki = Ki;
}

float PidController::getControlSignal(float currentState, float targetState)
{
  float currentError = targetState - currentState;

  float errorDerivative =
      (currentError - previousError) / this->timestepSeconds;

  this->errorIntegral += (currentError * this->timestepSeconds);

  if (this->params.windupLimitFactor != 1.0)
  {
    this->applyAntiIntegralWindupMechanism();
  }

  float controlSignal = (this->params.Kp * currentError) +
                        (this->params.Kd * errorDerivative) +
                        (this->params.Ki * this->errorIntegral);

  this->previousError = currentError;

  // Clamp the output between the min and max control signal values
  return std::clamp<float>(
      controlSignal, params.minControlSignal, params.maxControlSignal);
}

std::string PidController::Parameters::toString() const
{
  return format(
      "PID Control Parameters \n Kp: %.2f, Kd: %.2f, Ki: %.2f, dtMs: %u, "
      "Min: %.2f, Max: %.2f, Windup Factor:  %.2f",
      this->Kp,
      this->Kd,
      this->Ki,
      this->timestepMs,
      this->minControlSignal,
      this->maxControlSignal,
      this->windupLimitFactor);
}

void PidController::applyAntiIntegralWindupMechanism()
{
  float minIntegral = this->params.windupLimitFactor *
                      this->params.minControlSignal / this->params.Ki;
  float maxIntegral = this->params.windupLimitFactor *
                      this->params.maxControlSignal / this->params.Ki;
  this->errorIntegral =
      std::clamp<float>(this->errorIntegral, minIntegral, maxIntegral);
}