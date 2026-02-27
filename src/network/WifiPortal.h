#pragma once
#include <Arduino.h>
#include <WebServer.h>
#include <functional>
#include "Types.h"

class WifiPortal {
public:
  void begin();
  void tick(uint32_t nowMs, const Inputs& rawInputs, const Inputs& stableInputs, uint32_t lastTurnMs, uint32_t avgTurnMs, uint32_t turnCount);
  bool isActive() const { return _active; }
  void setCommandHandler(std::function<void(const String&)> handler) { _commandHandler = handler; }

private:
  void startAccessPoint();
  void setupServer();
  void stop();

  WebServer _server{80};
  bool _active = false;
  bool _clientConnected = false;
  bool _pendingStop = false;
  uint32_t _startMs = 0;

  Inputs _rawInputs;
  Inputs _stableInputs;
  uint32_t _lastTurnMs = 0;
  uint32_t _avgTurnMs = 0;
  uint32_t _turnCount = 0;
  std::function<void(const String&)> _commandHandler;
};
