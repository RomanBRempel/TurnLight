#pragma once
#include <Arduino.h>
#include "Types.h"
#include "Debounce.h"

class InputManager {
public:
  void begin();
  Inputs read(uint32_t nowMs);

private:
  DebouncedInput _brake;
  DebouncedInput _turn;

  bool readBrakeRaw() const;
  bool readTurnRaw() const;

  // If you wire divider to make HIGH=active, keep as true.
  // If later you use pullups/opto with inverted logic, flip these.
  static constexpr bool ACTIVE_HIGH = true;
};