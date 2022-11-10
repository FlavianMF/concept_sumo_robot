#include "motors.h"

#include <Arduino.h>

static int right_min_pwm = 1000;
static int right_middle_pwm = 1500;
static int right_max_pwm = 2000;
 
static int left_min_pwm = 1000;
static int left_middle_pwm = 1500;
static int left_max_pwm = 2000;

void setup_motors() {
  mcpwm_config_t pwm_config;

  pwm_config.frequency = 50;
  pwm_config.cmpr_a = 0;
  pwm_config.cmpr_b = 0;
  pwm_config.counter_mode = MCPWM_UP_COUNTER;
  pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
  
  ESP_ERROR_CHECK(mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, left_wheel_pwm_pin));
  ESP_ERROR_CHECK(mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0B, right_wheel_pwm_pin));
  ESP_ERROR_CHECK(mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config));
  ESP_ERROR_CHECK(mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, 1500));
  ESP_ERROR_CHECK(mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B, 1500));
}

void drive_motors(float left_duty, float right_duty) {

  if (left_duty > 0) {
    mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, map(left_duty, 0, 100, left_middle_pwm, left_max_pwm));
  } else if (left_duty < 0) {
    mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, map((left_duty * -1), 0, 100, left_middle_pwm, left_min_pwm));
  } else {
    mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, left_middle_pwm);
  }

  if (right_duty > 0) {
    mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B, map(right_duty, 0, 100, right_middle_pwm, right_max_pwm));
  } else if (right_duty < 0) {
    mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B, map((right_duty * -1), 0, 100, right_middle_pwm, right_min_pwm));
  } else {
    mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B, right_middle_pwm);
  }
  
}