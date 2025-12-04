#include "Data.h"
#include "BaseAgent.h"
#include "Agent.h"
#include "Emulator.h"
#include "MainAgent.h"

int main() {
  MainAgent zero;
  Emulator em(&zero);
  zero.emulator = &em;
  em.Run();
}