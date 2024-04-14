#pragma once
#include "Arduino.h"
#include <RGBLed.h>
#include <array>

/**
 * @brief Represents and encapsulates the "eyes" of the robot
 *
 * Abstracts away the underlying eye colour mechanicam - an RGB LED - and only
 * exposes a method for setting the eye colour
 *
 * Underlying Library: Uses the RGBLed library to control the RGB LED
 *
 */
class Eyes
{
 public:
  enum class Colour
  {
    off,
    red,
    green,
    blue,
    light_blue,
  };

  Eyes();

  /**
   * @brief Set the Colour object
   *
   * @param colour - the desired colour of the eyes
   */
  void setColour(Colour colour);

  /**
   * @brief Crossfade from one colour to another
   *
   * @param from - the starting colour
   * @param to - the ending colour
   * @param milliSeconds - the duration of the crossfade
   */
  void crossFade(Colour from, Colour to, int milliSeconds);

 private:
  // The pins the RGB LED is connected to. In the case of the robot, there are
  // two RGB LEDs wired togeher - each LED represents one eye
  int redPin{A0};
  int greenPin{A1};
  int bluePin{A2};

  // The RGB LED object
  RGBLed eyes;

  static constexpr int CROSS_FADE_STEPS{100};

  // RGB values for the various eye colours
  struct EyeColourRgbValues
  {
    static constexpr std::array<int, 3> OFF{0, 0, 0};
    static constexpr std::array<int, 3> RED{255, 0, 0};
    static constexpr std::array<int, 3> GREEN{0, 255, 0};
    static constexpr std::array<int, 3> BLUE{0, 0, 255};
    static constexpr std::array<int, 3> LIGHT_BLUE{173, 216, 255};
  };

  static std::array<int, 3> rgbValueFromEyeColour(Colour colour);
};
