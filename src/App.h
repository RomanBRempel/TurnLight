#pragma once
#include <Arduino.h>
#include "Types.h"
#include "inputs/InputManager.h"
#include "logic/ModeResolver.h"
#include "led/LedStrip.h"
#include "RuntimeConfig.h"
#include "network/WifiPortal.h"

class App {
public:
  void begin();
  void tick(uint32_t nowMs);
  void handleCommand(const String& command);

private:
  void handleSerial();
  void processCommand(char* line);
  void applyOutputSmoothing(uint32_t nowMs, uint8_t targetBrightness);

  InputManager _inputs;
  ModeResolver _resolver;
  LedStrip _strip;
  WifiPortal _wifiPortal;

  bool _overrideActive = false;
  ResolvedMode _overrideMode;
  uint32_t _bootMs = 0;
  bool _turnHoldActive = false;
  uint32_t _turnLastHighMs = 0;
  uint32_t _lastStatusMs = 0;
  Inputs _lastInputsRaw;
  Inputs _lastInputsStable;
  bool _turnHighActive = false;
  uint32_t _turnHighStartMs = 0;
  uint32_t _turnHighLastMs = 0;
  uint32_t _turnHighTotalMs = 0;
  uint32_t _turnHighCount = 0;
  uint32_t _turnHighAvgMs = 0;
  bool _outputSmoothingInit = false;
  uint32_t _outputSmoothingLastMs = 0;
  uint8_t _smoothedBrightness = 0;
  CRGB _smoothedFrame[LedLayout::LED_COUNT];
};