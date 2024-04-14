#include "eyes.hpp"

Eyes::Eyes() : eyes(redPin, greenPin, bluePin, RGBLed::COMMON_ANODE)
{
}

void Eyes::setColour(Colour colour)
{
  this->eyes.setColor(Eyes::rgbValueFromEyeColour(colour).data());
}

void Eyes::crossFade(Colour from, Colour to, int milliSeconds)
{
  this->eyes.crossFade(Eyes::rgbValueFromEyeColour(from).data(),
                       Eyes::rgbValueFromEyeColour(to).data(),
                       CROSS_FADE_STEPS,
                       milliSeconds);
}

std::array<int, 3> Eyes::rgbValueFromEyeColour(Colour colour)
{
  switch (colour)
  {
    case Colour::off:
      return EyeColourRgbValues::OFF;
    case Colour::red:
      return EyeColourRgbValues::RED;
    case Colour::green:
      return EyeColourRgbValues::GREEN;
    case Colour::blue:
      return EyeColourRgbValues::BLUE;
    case Colour::light_blue:
      return EyeColourRgbValues::LIGHT_BLUE;
  }
}
