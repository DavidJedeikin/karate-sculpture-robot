#pragma once
#include <string>

/**
 * @brief Singleton class that manages the serial communication used for logging
 *
 * Singleton class ensures only one instance of the class is created and
 * provides a global point of access to it
 *
 */
class SerialManager
{
 public:
  void write(std::string&& message);
  static SerialManager& getInstance();

  // Delete move and copy constructors
  SerialManager(SerialManager&&) = delete;
  SerialManager(SerialManager const&) = delete;

  // Delete move and copy assignment operators
  void operator=(SerialManager&&) = delete;
  void operator=(SerialManager const&) = delete;

 private:
  SerialManager();
};
