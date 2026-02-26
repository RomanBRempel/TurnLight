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
  void handleSerial();
  void processCommand(char* line);

  InputManager _inputs;
  ModeResolver _resolver;
  LedStrip _strip;

  bool _overrideActive = false;
  ResolvedMode _overrideMode;
  uint32_t _bootMs = 0;
  bool _turnHoldActive = false;
  uint32_t _turnLastHighMs = 0;
  uint32_t _lastStatusMs = 0;
};