#include "rgb.h"

RGBLed rgb(RED_RGB_PIN, GREEN_RGB_PIN, BLUE_RGB_PIN, RGBLed::COMMON_CATHODE);

void rgb_any_error(uint64_t *last_flash_time) {
  static bool color_flag = false;

  if (millis() > *last_flash_time + 500) {
    *last_flash_time = millis();
    color_flag = !color_flag;
    rgb.setColor(color_flag ? RGBLed::RED : RGBLed::YELLOW);
  }
}
