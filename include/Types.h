#pragma once
#include <Arduino.h>

enum class TurnDirection : uint8_t { CW, CCW };

struct Inputs {
  bool brake = false;
  bool turn  = false;
};

struct ResolvedMode {
  bool tailEnabled  = true;   // always on
  bool brakeEnabled = false;
  bool turnEnabled  = false;
};