#pragma once
#include <Arduino.h>

namespace Pins {
  static constexpr gpio_num_t LED_DATA = GPIO_NUM_4;

  static constexpr gpio_num_t IN_BRAKE = GPIO_NUM_5; // STOP (+12V -> divider -> 3.3V)
  static constexpr gpio_num_t IN_TURN  = GPIO_NUM_6; // TURN (+12V -> divider -> 3.3V)
}