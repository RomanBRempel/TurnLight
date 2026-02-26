#pragma once
#include <Arduino.h>
#include "Types.h"
#include "inputs/InputManager.h"
#include "logic/ModeResolver.h"
#include "led/LedStrip.h"

class App {
public:
  void begin();
  void tick(uint32_t nowMs);

private:
  InputManager _inputs;
  ModeResolver _resolver;
  LedStrip _strip;
};