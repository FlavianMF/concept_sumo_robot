#ifndef __MOVE_FUNCTIONS_H__
#define __MOVE_FUNCTIONS_H__

extern float accel_time;
extern float slow_time;

extern float linear_kp;
extern float linear_ki;
extern float linear_kd;

void forward_move(float linear_velocity, float angular_velocity);

void stop_motors();

#endif // __MOVE_FUNCTIONS_H__