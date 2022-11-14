#ifndef __MOTORS_H__
#define __MOTORS_H__

#include "driver/mcpwm.h"

#define right_wheel_pwm_pin 27
#define right_wheel_pwm_channel 3
#define right_wheel_pwm_frequency 50
#define right_wheel_pwm_resolution 16

#define left_wheel_pwm_pin 14
#define left_wheel_pwm_channel 4
#define left_wheel_pwm_frequency 50
#define left_wheel_pwm_resolution 16

extern float left_gain;
extern float right_gain;

/**
 * @brief Set the up motors
 * 
 */
void setup_motors();

/**
 * @brief drive left and right wheel with duty in percentage 
 *  
 * @param left_duty left wheel duty
 * @param right_duty right wheel duty
 */
void drive_motors(float left_duty, float right_duty);

#endif // __MOTORS_H__