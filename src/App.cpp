#include "App.h"
#include "led/AnimTail.h"
#include "led/AnimBrake.h"
#include "led/AnimTurn.h"

void App::begin() {
  _inputs.begin();
  _strip.begin();
}

void App::tick(uint32_t nowMs) {
  const Inputs in = _inputs.read(nowMs);
  const ResolvedMode mode = _resolver.resolve(in);

  CRGB* leds = _strip.leds();

  // Base: tail always
  if (mode.tailEnabled) {
    renderTail(leds);
  }

  // Brake layer: small+large overwrite
  if (mode.brakeEnabled) {
    renderBrakeSmallAndLarge(leds);
  }

  // Turn layer: mid ring overwrite (with blink envelope)
  if (mode.turnEnabled) {
    renderTurnMidRing(leds, nowMs);
  }

  _strip.show();
}