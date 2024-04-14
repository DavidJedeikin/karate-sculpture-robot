#pragma once

/**
 * @brief Class to map a linear range to another linear range
 *
 */
class LinearMap
{
 public:
  struct Params
  {
    float inputMin;
    float inputMax;
    float outputMin;
    float outputMax;
  };

  LinearMap(Params params);

  /**
   * @brief Get the output value for a given input value
   *
   * @param input The input value
   * @return float The output value
   */
  [[nodiscard]] float getOutput(float input) const;

 private:
  Params params;
  float m;
  float c;
};