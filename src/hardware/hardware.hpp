#pragma once
#include "eyes.hpp"
#include "joints.hpp"
#include "sonarArray.hpp"
#include "switch.hpp"

/**
 * @brief Structure to hold all the hardware components
 *
 */
struct Hardware
{
  Eyes eyes;
  Joints joints;
  SonarArray sonarArray;
  Switch modeSwitch;
};