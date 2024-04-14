#include "SerialManager.hpp"
#include "Uart.h"

void SerialManager::write(std::string&& message)
{
  Serial.printf("%s\r\n", std::move(message).c_str());
}

SerialManager& SerialManager::getInstance()
{
  static SerialManager logger;
  return logger;
}

SerialManager::SerialManager()
{
  Serial.begin(115200);

  // Wait while serial is initialised
  while (!Serial)
  {
    ;
  };
}