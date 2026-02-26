#include "App.h"
#include "led/AnimTail.h"
#include "led/AnimBrake.h"
#include "led/AnimTurn.h"
#include "led/AnimCommon.h"
#include "LedLayout.h"
#include "Config.h"
#include <ctype.h>
#include <string.h>

namespace {
  void toLowerInPlace(char* s) {
    for (; *s; ++s) {
      *s = static_cast<char>(tolower(static_cast<unsigned char>(*s)));
    }
  }

  char* trimInPlace(char* s) {
    while (*s && isspace(static_cast<unsigned char>(*s))) {
      ++s;
    }
    if (*s == '\0') {
      return s;
    }
    char* end = s + strlen(s) - 1;
    while (end > s && isspace(static_cast<unsigned char>(*end))) {
      *end = '\0';
      --end;
    }
    return s;
  }

  uint8_t ringBreathValue(uint32_t nowMs, uint32_t offsetMs) {
    const uint32_t period = Config::STARTUP_BREATH_PERIOD_MS;
    if (period == 0) {
      return 0;
    }
    const uint32_t t = (nowMs + offsetMs) % period;
    const uint8_t phase = static_cast<uint8_t>((t * 255UL) / period);
    return sin8(phase);
  }

  void renderStartupBreathBlue(CRGB* leds, uint32_t nowMs) {
    const uint32_t period = Config::STARTUP_BREATH_PERIOD_MS;
    if (period == 0) {
      return;
    }

    const uint32_t phase0 = (nowMs % period);
    const uint32_t phase1 = (phase0 + (period / 3U)) % period;
    const uint32_t phase2 = (phase0 + (period * 2U / 3U)) % period;

    const uint8_t vLarge = sin8(static_cast<uint8_t>((phase0 * 255UL) / period));
    const uint8_t vMid   = sin8(static_cast<uint8_t>((phase1 * 255UL) / period));
    const uint8_t vSmall = sin8(static_cast<uint8_t>((phase2 * 255UL) / period));

    const uint8_t range = Config::STARTUP_BREATH_MAX_BRIGHT - Config::STARTUP_BREATH_MIN_BRIGHT;
    const uint8_t bLarge = Config::STARTUP_BREATH_MIN_BRIGHT + scale8(vLarge, range);
    const uint8_t bMid   = Config::STARTUP_BREATH_MIN_BRIGHT + scale8(vMid, range);
    const uint8_t bSmall = Config::STARTUP_BREATH_MIN_BRIGHT + scale8(vSmall, range);

    for (uint16_t i = 0; i < LedLayout::RING_LARGE_COUNT; i++) {
      leds[LedLayout::RING_LARGE_START + i] = CRGB(0, 0, bLarge);
    }
    for (uint16_t i = 0; i < LedLayout::RING_MID_COUNT; i++) {
      leds[LedLayout::RING_MID_START + i] = CRGB(0, 0, bMid);
    }
    for (uint16_t i = 0; i < LedLayout::RING_SMALL_COUNT; i++) {
      leds[LedLayout::RING_SMALL_START + i] = CRGB(0, 0, bSmall);
    }
  }

  void renderStartupFadeToTail(CRGB* leds, uint32_t fadeElapsedMs) {
    const uint32_t duration = Config::STARTUP_FADE_MS;
    const uint8_t mix = (duration == 0) ? 255 : static_cast<uint8_t>((fadeElapsedMs * 255UL) / duration);

    const CRGB blue = CRGB(0, 0, Config::STARTUP_BREATH_MAX_BRIGHT);
    const CRGB tailSmall = colorTail(Config::TAIL_SMALL_BRIGHT);
    const CRGB tailMid   = colorTail(Config::TAIL_MID_BRIGHT);
    const CRGB tailLarge = colorTail(Config::TAIL_LARGE_BRIGHT);

    const CRGB smallColor = blend(blue, tailSmall, mix);
    const CRGB midColor   = blend(blue, tailMid, mix);
    const CRGB largeColor = blend(blue, tailLarge, mix);

    for (uint16_t i = 0; i < LedLayout::RING_LARGE_COUNT; i++) {
      leds[LedLayout::RING_LARGE_START + i] = largeColor;
    }
    for (uint16_t i = 0; i < LedLayout::RING_MID_COUNT; i++) {
      leds[LedLayout::RING_MID_START + i] = midColor;
    }
    for (uint16_t i = 0; i < LedLayout::RING_SMALL_COUNT; i++) {
      leds[LedLayout::RING_SMALL_START + i] = smallColor;
    }
  }

  void renderStartupFlashWhite(CRGB* leds) {
    fill_solid(leds, LedLayout::LED_COUNT, CRGB::White);
  }

  bool isTurnWaveSafeToEnd(uint32_t nowMs, uint8_t activeRings) {
    const uint32_t cycle = turnWaveCycleMs(activeRings);
    if (cycle == 0) {
      return true;
    }
    const uint32_t phase = nowMs % cycle;
    const uint32_t step = Config::TURN_RING_STEP_MS;
    if (phase >= (step * activeRings)) {
      return true;
    }
    return phase < Config::TURN_RING_BOUNDARY_WINDOW_MS;
  }
}

void App::begin() {
  _inputs.begin();
  _strip.begin();
  _bootMs = millis();
  _lastStatusMs = _bootMs;
  Serial.println("Ready. Waiting for command (type: help)");
}

void App::tick(uint32_t nowMs) {
  if (nowMs - _bootMs < 10000U) {
    if ((uint32_t)(nowMs - _lastStatusMs) >= 2000U) {
      _lastStatusMs = nowMs;
      Serial.println("Waiting for command (type: help)");
    }
  }
  const uint32_t startupTotal = Config::STARTUP_BREATH_MS + Config::STARTUP_FLASH_MS + Config::STARTUP_FADE_MS;
  if (nowMs - _bootMs < startupTotal) {
    _strip.clear();
    const uint32_t elapsed = nowMs - _bootMs;
    if (elapsed < Config::STARTUP_BREATH_MS) {
      renderStartupBreathBlue(_strip.leds(), nowMs);
    } else if (elapsed < (Config::STARTUP_BREATH_MS + Config::STARTUP_FLASH_MS)) {
      renderStartupFlashWhite(_strip.leds());
    } else {
      renderStartupFadeToTail(
        _strip.leds(),
        elapsed - Config::STARTUP_BREATH_MS - Config::STARTUP_FLASH_MS
      );
    }
    _strip.show();
    return;
  }

  handleSerial();
  const Inputs in = _inputs.read(nowMs);

  if (in.turn) {
    _turnHoldActive = true;
    _turnLastHighMs = nowMs;
  } else if (_turnHoldActive) {
    if ((uint32_t)(nowMs - _turnLastHighMs) >= Config::TURN_OFF_HOLD_MS) {
      const bool turnOnlyInputs = !in.brake;
      const uint8_t activeRings = turnOnlyInputs ? 3 : 2;
      if (isTurnWaveSafeToEnd(nowMs, activeRings)) {
        _turnHoldActive = false;
      }
    }
  }

  Inputs stableIn = in;
  stableIn.turn = _turnHoldActive;

  ResolvedMode mode = _resolver.resolve(stableIn);
  if (_overrideActive) {
    mode = _overrideMode;
  }

  const bool tailOnly = mode.tailEnabled && !mode.brakeEnabled && !mode.turnEnabled;
  _strip.setBrightness(tailOnly ? Config::LED_BRIGHTNESS_TAIL : Config::LED_BRIGHTNESS_MAX);
  const bool turnOnly = mode.turnEnabled && !mode.brakeEnabled;
  const uint8_t turnActiveRings = turnOnly ? 3 : 2;
  const uint8_t tailScale = turnOnly ? 128 : 255;

  CRGB* leds = _strip.leds();

  // Base: tail always
  if (mode.tailEnabled) {
    renderTail(leds, tailScale);
  }

  // Brake layer: small+large overwrite
  if (mode.brakeEnabled) {
    renderBrakeSmallAndLarge(leds);
  }

  // Turn layer: mid ring overwrite (with blink envelope)
  if (mode.turnEnabled) {
    renderTurnRingsWave(leds, nowMs, turnActiveRings);
  }

  _strip.show();
}

void App::handleSerial() {
  static char buffer[64];
  static size_t len = 0;

  while (Serial.available() > 0) {
    char c = static_cast<char>(Serial.read());
    if (c == '\n' || c == '\r') {
      buffer[len] = '\0';
      processCommand(buffer);
      len = 0;
      continue;
    }

    if (len < sizeof(buffer) - 1) {
      buffer[len++] = c;
    } else {
      len = 0;
    }
  }
}

void App::processCommand(char* line) {
  char* cmd = trimInPlace(line);
  if (*cmd == '\0') {
    return;
  }

  toLowerInPlace(cmd);

  if (strcmp(cmd, "help") == 0 || strcmp(cmd, "?") == 0) {
    Serial.println("Commands: mode auto|tail|brake|turn|all|test|off");
    return;
  }

  if (strncmp(cmd, "mode", 4) != 0) {
    Serial.println("Unknown command. Try: help");
    return;
  }

  char* arg = trimInPlace(cmd + 4);
  if (*arg == '\0') {
    Serial.println("Usage: mode auto|tail|brake|turn|all|test|off");
    return;
  }

  if (strcmp(arg, "auto") == 0) {
    _overrideActive = false;
    Serial.println("Mode: auto (inputs)");
    return;
  }

  _overrideActive = true;
  ResolvedMode m;
  m.tailEnabled = false;
  m.brakeEnabled = false;
  m.turnEnabled = false;

  if (strcmp(arg, "tail") == 0) {
    m.tailEnabled = true;
    Serial.println("Mode: tail");
  } else if (strcmp(arg, "brake") == 0) {
    m.tailEnabled = true;
    m.brakeEnabled = true;
    Serial.println("Mode: brake");
  } else if (strcmp(arg, "turn") == 0) {
    m.tailEnabled = true;
    m.turnEnabled = true;
    Serial.println("Mode: turn");
  } else if (strcmp(arg, "all") == 0 || strcmp(arg, "test") == 0) {
    m.tailEnabled = true;
    m.brakeEnabled = true;
    m.turnEnabled = true;
    Serial.println("Mode: test (brake + turn)");
  } else if (strcmp(arg, "off") == 0) {
    Serial.println("Mode: off");
  } else {
    _overrideActive = false;
    Serial.println("Unknown mode. Try: help");
    return;
  }

  _overrideMode = m;
}