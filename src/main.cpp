#include "application/application.hpp"

// Avoid falling prey to the setup method as it usually leads to poor software
// patterns and code quality. Instead, use standard c++ object oriented
// principals and StateMachines where possible
void setup()
{
}

void loop()
{
  Application application;
  application.run();
}
