#pragma once
#include <Arduino.h>

class DebouncedInput {
public:
  explicit DebouncedInput(uint16_t debounceMs = 30)
  : _debounceMs(debounceMs) {}

  // Call with raw digitalRead value (already normalized to true=active)
  bool update(bool raw, uint32_t nowMs);

  bool value() const { return _stable; }

private:
  uint16_t _debounceMs;
  bool _stable = false;
  bool _lastRaw = false;
  uint32_t _lastChange = 0;
};