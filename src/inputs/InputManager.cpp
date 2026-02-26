#include "InputManager.h"
#include "Pins.h"
#include "Config.h"

void InputManager::begin() {
  _brake = DebouncedInput(Config::INPUT_DEBOUNCE_MS);
  _turn  = DebouncedInput(Config::INPUT_DEBOUNCE_MS);

  // With a divider from 12V -> GPIO, typically you want plain INPUT.
  // (Pullups can distort divider in some cases.)
  pinMode(Pins::IN_BRAKE, INPUT);
  pinMode(Pins::IN_TURN, INPUT);
}

bool InputManager::readBrakeRaw() const {
  int v = digitalRead(Pins::IN_BRAKE);
  return ACTIVE_HIGH ? (v == HIGH) : (v == LOW);
}

bool InputManager::readTurnRaw() const {
  int v = digitalRead(Pins::IN_TURN);
  return ACTIVE_HIGH ? (v == HIGH) : (v == LOW);
}

Inputs InputManager::read(uint32_t nowMs) {
  Inputs in;
  in.brake = _brake.update(readBrakeRaw(), nowMs);
  in.turn  = _turn.update(readTurnRaw(), nowMs);
  return in;
}