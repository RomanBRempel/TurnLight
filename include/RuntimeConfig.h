#pragma once
#include <Arduino.h>
#include "Config.h"

namespace RuntimeConfig {
  struct Data {
    uint8_t ledBrightnessMax;
    uint8_t ledBrightnessTail;

    uint16_t startupBreathMs;
    uint16_t startupFlashMs;
    uint16_t startupFadeMs;
    uint16_t startupBreathPeriodMs;
    uint8_t startupBreathMaxBright;
    uint8_t startupBreathMinBright;
    uint8_t startupBreathBlueR;
    uint8_t startupBreathBlueG;

    uint8_t tailSmallBright;
    uint8_t tailMidBright;
    uint8_t tailLargeBright;

    uint8_t brakeBright;

    uint16_t turnRingStepMs;
    uint16_t turnRingCyclePauseMs;
    uint8_t turnRingBright;
    uint16_t turnOffHoldMs;

    uint8_t turnOrangeR;
    uint8_t turnOrangeG;
    uint8_t turnOrangeB;
  };

  Data defaults();
  Data& get();

  void load();
  void save();
  void resetToDefaults();
}
