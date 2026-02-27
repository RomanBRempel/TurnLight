#include "WifiPortal.h"
#include <WiFi.h>
#include <ArduinoOTA.h>
#include "RuntimeConfig.h"

namespace {
  const char* kApSsid = "TurnLight-Setup";

  String boolToJson(bool v) {
    return v ? "true" : "false";
  }

  String configToJson(const RuntimeConfig::Data& cfg) {
    String json = "{";
    json += "\"ledBrightnessMax\":" + String(cfg.ledBrightnessMax) + ",";
    json += "\"ledBrightnessTail\":" + String(cfg.ledBrightnessTail) + ",";
    json += "\"startupBreathMs\":" + String(cfg.startupBreathMs) + ",";
    json += "\"startupFlashMs\":" + String(cfg.startupFlashMs) + ",";
    json += "\"startupFadeMs\":" + String(cfg.startupFadeMs) + ",";
    json += "\"startupBreathPeriodMs\":" + String(cfg.startupBreathPeriodMs) + ",";
    json += "\"startupBreathMaxBright\":" + String(cfg.startupBreathMaxBright) + ",";
    json += "\"startupBreathMinBright\":" + String(cfg.startupBreathMinBright) + ",";
    json += "\"startupBreathBlueR\":" + String(cfg.startupBreathBlueR) + ",";
    json += "\"startupBreathBlueG\":" + String(cfg.startupBreathBlueG) + ",";
    json += "\"tailSmallBright\":" + String(cfg.tailSmallBright) + ",";
    json += "\"tailMidBright\":" + String(cfg.tailMidBright) + ",";
    json += "\"tailLargeBright\":" + String(cfg.tailLargeBright) + ",";
    json += "\"brakeBright\":" + String(cfg.brakeBright) + ",";
    json += "\"turnRingStepMs\":" + String(cfg.turnRingStepMs) + ",";
    json += "\"turnRingCyclePauseMs\":" + String(cfg.turnRingCyclePauseMs) + ",";
    json += "\"turnRingBright\":" + String(cfg.turnRingBright) + ",";
    json += "\"turnOffHoldMs\":" + String(cfg.turnOffHoldMs) + ",";
    json += "\"turnOrangeR\":" + String(cfg.turnOrangeR) + ",";
    json += "\"turnOrangeG\":" + String(cfg.turnOrangeG) + ",";
    json += "\"turnOrangeB\":" + String(cfg.turnOrangeB);
    json += "}";
    return json;
  }

  String statusToJson(const Inputs& rawInputs, const Inputs& stableInputs, uint32_t lastTurnMs, uint32_t avgTurnMs, uint32_t turnCount) {
    String json = "{";
    json += "\"raw\":{\"brake\":" + boolToJson(rawInputs.brake) + ",\"turn\":" + boolToJson(rawInputs.turn) + "},";
    json += "\"stable\":{\"brake\":" + boolToJson(stableInputs.brake) + ",\"turn\":" + boolToJson(stableInputs.turn) + "},";
    json += "\"turn\":{\"lastMs\":" + String(lastTurnMs) + ",\"avgMs\":" + String(avgTurnMs) + ",\"count\":" + String(turnCount) + "}";
    json += "}";
    return json;
  }

  void applyArg(WebServer& server, const char* key, uint16_t& value) {
    if (server.hasArg(key)) {
      value = static_cast<uint16_t>(server.arg(key).toInt());
    }
  }

  void applyArg(WebServer& server, const char* key, uint8_t& value) {
    if (server.hasArg(key)) {
      value = static_cast<uint8_t>(server.arg(key).toInt());
    }
  }
}

void WifiPortal::begin() {
  startAccessPoint();
  setupServer();

  ArduinoOTA.setHostname("turnlight");
  ArduinoOTA.begin();

  _active = true;
  _clientConnected = false;
  _pendingStop = false;
  _startMs = millis();
}

void WifiPortal::tick(uint32_t nowMs, const Inputs& rawInputs, const Inputs& stableInputs, uint32_t lastTurnMs, uint32_t avgTurnMs, uint32_t turnCount) {
  if (!_active) {
    return;
  }

  _rawInputs = rawInputs;
  _stableInputs = stableInputs;
  _lastTurnMs = lastTurnMs;
  _avgTurnMs = avgTurnMs;
  _turnCount = turnCount;

  if (_pendingStop) {
    stop();
    return;
  }

  if (!_clientConnected) {
    if (WiFi.softAPgetStationNum() > 0) {
      _clientConnected = true;
    } else if ((uint32_t)(nowMs - _startMs) >= 120000U) {
      stop();
      return;
    }
  }

  _server.handleClient();
  if (_clientConnected) {
    ArduinoOTA.handle();
  }
}

void WifiPortal::startAccessPoint() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(kApSsid);
}

void WifiPortal::setupServer() {
  _server.on("/", HTTP_GET, [this]() {
    String html;
    html += "<!doctype html><html><head><meta charset='utf-8'>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<title>TurnLight</title>";
    html += "<style>body{font-family:Arial;padding:16px}input{width:120px}</style>";
    html += "</head><body>";
    html += "<h2>TurnLight</h2>";
    html += "<div id='status'>Loading...</div>";
    html += "<div id='turn'>Loading...</div>";
    html += "<h3>Test</h3>";
    html += "<div>";
    html += "<button onclick=\"setMode('auto')\">Auto</button> ";
    html += "<button onclick=\"setMode('tail')\">Tail</button> ";
    html += "<button onclick=\"setMode('brake')\">Brake</button> ";
    html += "<button onclick=\"setMode('turn')\">Turn</button> ";
    html += "<button onclick=\"setMode('test')\">Test</button> ";
    html += "<button onclick=\"setMode('off')\">Off</button>";
    html += "</div>";
    html += "<h3>Config</h3>";
    html += "<form id='cfg'></form>";
    html += "<button type='button' onclick=\"saveCfg()\">Save</button> ";
    html += "<button type='button' onclick=\"loadConfig()\">Reload</button> ";
    html += "<button type='button' onclick=\"exitWifi()\">Exit WiFi</button>";
    html += "<p>OTA: use host <b>turnlight</b> or IP 192.168.4.1</p>";
    html += "<script>\n";
    html += "async function loadStatus(){\n";
    html += "const s=await fetch('/status').then(r=>r.json());\n";
    html += "document.getElementById('status').innerText='RAW brake='+s.raw.brake+' turn='+s.raw.turn+' | STABLE brake='+s.stable.brake+' turn='+s.stable.turn;\n";
    html += "document.getElementById('turn').innerText='Turn high: last='+s.turn.lastMs+' ms, avg='+s.turn.avgMs+' ms ('+s.turn.count+')';\n";
    html += "}\n";
    html += "async function loadConfig(){\n";
    html += "const c=await fetch('/config').then(r=>r.json());\n";
    html += "const keys=Object.keys(c);\n";
    html += "let f='';\n";
    html += "for(const k of keys){f+=`<div>${k}: <input name='${k}' value='${c[k]}'></div>`;}\n";
    html += "document.getElementById('cfg').innerHTML=f;\n";
    html += "}\n";
    html += "async function saveCfg(){\n";
    html += "const data=new URLSearchParams(new FormData(document.getElementById('cfg')));\n";
    html += "await fetch('/config',{method:'POST',body:data});\n";
    html += "await fetch('/save');\n";
    html += "}\n";
    html += "function setMode(m){fetch('/mode?value='+m);}\n";
    html += "function exitWifi(){fetch('/exit');}\n";
    html += "setInterval(loadStatus,1000);loadStatus();loadConfig();\n";
    html += "</script></body></html>";

    _server.send(200, "text/html", html);
  });

  _server.on("/status", HTTP_GET, [this]() {
    _server.send(200, "application/json", statusToJson(_rawInputs, _stableInputs, _lastTurnMs, _avgTurnMs, _turnCount));
  });
  _server.on("/mode", HTTP_GET, [this]() {
    if (_commandHandler && _server.hasArg("value")) {
      _commandHandler(String("mode ") + _server.arg("value"));
    }
    _server.send(200, "application/json", "{\"ok\":true}");
  });

  _server.on("/config", HTTP_GET, [this]() {
    _server.send(200, "application/json", configToJson(RuntimeConfig::get()));
  });

  _server.on("/config", HTTP_POST, [this]() {
    RuntimeConfig::Data& cfg = RuntimeConfig::get();
    applyArg(_server, "ledBrightnessMax", cfg.ledBrightnessMax);
    applyArg(_server, "ledBrightnessTail", cfg.ledBrightnessTail);

    applyArg(_server, "startupBreathMs", cfg.startupBreathMs);
    applyArg(_server, "startupFlashMs", cfg.startupFlashMs);
    applyArg(_server, "startupFadeMs", cfg.startupFadeMs);
    applyArg(_server, "startupBreathPeriodMs", cfg.startupBreathPeriodMs);
    applyArg(_server, "startupBreathMaxBright", cfg.startupBreathMaxBright);
    applyArg(_server, "startupBreathMinBright", cfg.startupBreathMinBright);
    applyArg(_server, "startupBreathBlueR", cfg.startupBreathBlueR);
    applyArg(_server, "startupBreathBlueG", cfg.startupBreathBlueG);

    applyArg(_server, "tailSmallBright", cfg.tailSmallBright);
    applyArg(_server, "tailMidBright", cfg.tailMidBright);
    applyArg(_server, "tailLargeBright", cfg.tailLargeBright);

    applyArg(_server, "brakeBright", cfg.brakeBright);

    applyArg(_server, "turnRingStepMs", cfg.turnRingStepMs);
    applyArg(_server, "turnRingCyclePauseMs", cfg.turnRingCyclePauseMs);
    applyArg(_server, "turnRingBright", cfg.turnRingBright);
    applyArg(_server, "turnOffHoldMs", cfg.turnOffHoldMs);

    applyArg(_server, "turnOrangeR", cfg.turnOrangeR);
    applyArg(_server, "turnOrangeG", cfg.turnOrangeG);
    applyArg(_server, "turnOrangeB", cfg.turnOrangeB);

    _server.send(200, "application/json", "{\"ok\":true}");
  });

  _server.on("/save", HTTP_GET, [this]() {
    RuntimeConfig::save();
    _server.send(200, "text/plain", "saved");
  });

  _server.on("/exit", HTTP_GET, [this]() {
    _server.send(200, "text/plain", "bye");
    _pendingStop = true;
  });

  _server.begin();
}

void WifiPortal::stop() {
  _server.stop();
  WiFi.softAPdisconnect(true);
  WiFi.mode(WIFI_OFF);
  _active = false;
}
