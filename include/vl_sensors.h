#ifndef __VL_SENSORS_H__
#define __VL_SENSORS_H__
#include "Adafruit_VL53L0X.h"

#define front_xshut_pin 5
#define left_xshut_pin 18
#define right_xshut_pin 17

#define front_sensor_id 0x29
#define left_sensor_id 0x30
#define right_sensor_id 0x32

extern Adafruit_VL53L0X front_sensor;

extern VL53L0X_RangingMeasurementData_t front_sensor_measure;

void setup_vl_sensors();

int read_front_sensor();

#endif // __VL_SENSORS_H__