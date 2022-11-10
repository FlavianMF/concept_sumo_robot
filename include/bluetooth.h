#ifndef __BLUETOOTH_H__
#define __BLUETOOTH_H__

#include <BluetoothSerial.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

extern BluetoothSerial bluetooth;

/**
 * @brief Callback function of bluetooth events
 * 
 */
void bluetooth_callback(void);

/**
 * @brief Set the up bluetooth object
 * 
 */
void setup_bluetooth(void);

#endif // __BLUETOOTH_H__