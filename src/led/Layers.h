#pragma once
#include <FastLED.h>

// For now we "overwrite" pixels per layer in a controlled order.
// Tail -> Brake -> Turn. (Turn overwrites mid ring only)