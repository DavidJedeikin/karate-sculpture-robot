#include "linearMap.hpp"
#include <algorithm>

LinearMap::LinearMap(Params params)
    : params(params),
      // Calculating the linear constants
      // m = outputRange / inputRange
      // c = outputMax - m * inputMax
      m((params.outputMax - params.outputMin) /
        (params.inputMax - params.inputMin)),
      c(params.outputMax - this->m * params.inputMax)
{
}

float LinearMap::getOutput(float input) const
{
  // Linear equation:
  // Y = m * X + c, where:
  // -> Y is the output value
  // -> X is the input value
  // -> m is the slope
  // -> c is the y-intercept
  return std::clamp(this->m * input + this->c,
                    this->params.outputMin,
                    this->params.outputMax);
}
