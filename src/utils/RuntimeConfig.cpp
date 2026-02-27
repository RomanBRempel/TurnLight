#include "RuntimeConfig.h"
#include <Preferences.h>

namespace RuntimeConfig {
  static Data data;
  static Preferences prefs;

  Data defaults() {
    Data d;
    d.ledBrightnessMax = Config::LED_BRIGHTNESS_MAX;
    d.ledBrightnessTail = Config::LED_BRIGHTNESS_TAIL;

    d.startupBreathMs = Config::STARTUP_BREATH_MS;
    d.startupFlashMs = Config::STARTUP_FLASH_MS;
    d.startupFadeMs = Config::STARTUP_FADE_MS;
    d.startupBreathPeriodMs = Config::STARTUP_BREATH_PERIOD_MS;
    d.startupBreathMaxBright = Config::STARTUP_BREATH_MAX_BRIGHT;
    d.startupBreathMinBright = Config::STARTUP_BREATH_MIN_BRIGHT;
    d.startupBreathBlueR = Config::STARTUP_BREATH_BLUE_R;
    d.startupBreathBlueG = Config::STARTUP_BREATH_BLUE_G;

    d.tailSmallBright = Config::TAIL_SMALL_BRIGHT;
    d.tailMidBright = Config::TAIL_MID_BRIGHT;
    d.tailLargeBright = Config::TAIL_LARGE_BRIGHT;

    d.brakeBright = Config::BRAKE_BRIGHT;

    d.turnRingStepMs = Config::TURN_RING_STEP_MS;
    d.turnRingCyclePauseMs = Config::TURN_RING_CYCLE_PAUSE_MS;
    d.turnRingBright = Config::TURN_RING_BRIGHT;
    d.turnOffHoldMs = Config::TURN_OFF_HOLD_MS;

    d.turnOrangeR = Config::TURN_ORANGE_R;
    d.turnOrangeG = Config::TURN_ORANGE_G;
    d.turnOrangeB = Config::TURN_ORANGE_B;

    return d;
  }

  Data& get() {
    return data;
  }

  void load() {
    data = defaults();
    if (!prefs.begin("turnlight", true)) {
      return;
    }

    data.ledBrightnessMax = prefs.getUChar("lbmax", data.ledBrightnessMax);
    data.ledBrightnessTail = prefs.getUChar("lbtail", data.ledBrightnessTail);

    data.startupBreathMs = prefs.getUShort("sbms", data.startupBreathMs);
    data.startupFlashMs = prefs.getUShort("sfms", data.startupFlashMs);
    data.startupFadeMs = prefs.getUShort("sfdms", data.startupFadeMs);
    data.startupBreathPeriodMs = prefs.getUShort("sbpms", data.startupBreathPeriodMs);
    data.startupBreathMaxBright = prefs.getUChar("sbmax", data.startupBreathMaxBright);
    data.startupBreathMinBright = prefs.getUChar("sbmin", data.startupBreathMinBright);
    data.startupBreathBlueR = prefs.getUChar("sbr", data.startupBreathBlueR);
    data.startupBreathBlueG = prefs.getUChar("sbg", data.startupBreathBlueG);

    data.tailSmallBright = prefs.getUChar("tsb", data.tailSmallBright);
    data.tailMidBright = prefs.getUChar("tmb", data.tailMidBright);
    data.tailLargeBright = prefs.getUChar("tlb", data.tailLargeBright);

    data.brakeBright = prefs.getUChar("bb", data.brakeBright);

    data.turnRingStepMs = prefs.getUShort("trsm", data.turnRingStepMs);
    data.turnRingCyclePauseMs = prefs.getUShort("trpm", data.turnRingCyclePauseMs);
    data.turnRingBright = prefs.getUChar("trb", data.turnRingBright);
    data.turnOffHoldMs = prefs.getUShort("tohm", data.turnOffHoldMs);

    data.turnOrangeR = prefs.getUChar("tor", data.turnOrangeR);
    data.turnOrangeG = prefs.getUChar("tog", data.turnOrangeG);
    data.turnOrangeB = prefs.getUChar("tob", data.turnOrangeB);

    prefs.end();
  }

  void save() {
    if (!prefs.begin("turnlight", false)) {
      return;
    }

    prefs.putUChar("lbmax", data.ledBrightnessMax);
    prefs.putUChar("lbtail", data.ledBrightnessTail);

    prefs.putUShort("sbms", data.startupBreathMs);
    prefs.putUShort("sfms", data.startupFlashMs);
    prefs.putUShort("sfdms", data.startupFadeMs);
    prefs.putUShort("sbpms", data.startupBreathPeriodMs);
    prefs.putUChar("sbmax", data.startupBreathMaxBright);
    prefs.putUChar("sbmin", data.startupBreathMinBright);
    prefs.putUChar("sbr", data.startupBreathBlueR);
    prefs.putUChar("sbg", data.startupBreathBlueG);

    prefs.putUChar("tsb", data.tailSmallBright);
    prefs.putUChar("tmb", data.tailMidBright);
    prefs.putUChar("tlb", data.tailLargeBright);

    prefs.putUChar("bb", data.brakeBright);

    prefs.putUShort("trsm", data.turnRingStepMs);
    prefs.putUShort("trpm", data.turnRingCyclePauseMs);
    prefs.putUChar("trb", data.turnRingBright);
    prefs.putUShort("tohm", data.turnOffHoldMs);

    prefs.putUChar("tor", data.turnOrangeR);
    prefs.putUChar("tog", data.turnOrangeG);
    prefs.putUChar("tob", data.turnOrangeB);

    prefs.end();
  }

  void resetToDefaults() {
    data = defaults();
  }
}
