#include <Arduino.h>
#include "App.h"

static App app;

void setup() {
  Serial.begin(115200);
  delay(200);
  app.begin();
}

void loop() {
  app.tick(millis());
  // small yield; WS2812 show already blocks a bit
  delay(1);
}