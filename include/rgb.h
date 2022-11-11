#ifndef __RGB_H__
#define __RGB_H__

#include <RGBLed.h>

#define RED_RGB_PIN 16
#define GREEN_RGB_PIN 19
#define BLUE_RGB_PIN 12

extern RGBLed rgb;
// RGBLed rgb(RED_RGB_PIN, GREEN_RGB_PIN, BLUE_RGB_PIN, RGBLed::COMMON_CATHODE);

void rgb_any_error(uint64_t *last_flash_time);

#endif // __RGB_H__