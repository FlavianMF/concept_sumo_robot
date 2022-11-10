#ifndef __RGB_H__
#define __RGB_H__

#include <RGBLed.h>

#define RED_RGB_PIN 19
#define GREEN_RGB_PIN 16
#define BLUE_RGB_PIN 12

RGBLed rgb(RED_RGB_PIN, GREEN_RGB_PIN, BLUE_RGB_PIN, RGBLed::COMMON_CATHODE);

#endif // __RGB_H__