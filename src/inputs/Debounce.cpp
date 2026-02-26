#include "Debounce.h"

bool DebouncedInput::update(bool raw, uint32_t nowMs) {
  if (raw != _lastRaw) {
    _lastRaw = raw;
    _lastChange = nowMs;
  }

  if ((uint32_t)(nowMs - _lastChange) >= _debounceMs) {
    _stable = _lastRaw;
  }
  return _stable;
}