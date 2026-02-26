#pragma once
#include <Arduino.h>

#ifndef LOG_ENABLED
#define LOG_ENABLED 1
#endif

#if LOG_ENABLED
  #define LOGI(...) Serial.printf(__VA_ARGS__)
#else
  #define LOGI(...)
#endif