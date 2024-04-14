#include "application.hpp"
#include "application/danceState.hpp"
#include "application/trackingState.hpp"
#include "logging/log.hpp"
#include <memory>

Application::Application()
    : hardware(std::make_shared<Hardware>()),
      danceState(std::make_unique<DanceState>(this->hardware)),
      trackingState(std::make_unique<TrackingState>(this->hardware)),
      currentState(this->danceState.get())
{
  LOG_INFO("Starting Application in %s!", this->currentState->name());

  this->hardware->eyes.setColour(Eyes::Colour::light_blue);
}

void Application::run()
{
  while (true)
  {
    auto* desiredState = this->getDesiredState();
    if (desiredState == nullptr)
    {
      LOG_WARN("%s", "Desired state is a nullptr");
      return;
    }

    if (this->currentState != desiredState)
    {
      this->currentState = desiredState;
      this->currentState->enter();
    }
    else
    {
      this->currentState->runOnce();
    }
  }
}

IState* Application::getDesiredState()
{
  Switch::State currentSwitchState = this->hardware->modeSwitch.getState();
  return currentSwitchState == Switch::State::on ? this->danceState.get()
                                                 : this->trackingState.get();
}
