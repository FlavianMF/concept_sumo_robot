#include "bluetooth.h"

BluetoothSerial bluetooth;

static const char *TAG = "bluetooth";

void bluetooth_callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
  switch (event) {
    case ESP_SPP_SRV_OPEN_EVT:
      ESP_LOGV(TAG, "Bluetooth are connected");
      bluetooth.println(F("Welcome to GerOLDin firmware"));
      bluetooth.println(F("Press ? to discover more informations"));
      break;
    case ESP_SPP_CLOSE_EVT:
      ESP_LOGV (TAG, "Bluetooth are disconnected");
      break;
    // case ESP_SPP_DATA_IND_EVT:
    //   Log.info(F("[%s] Reading bluetooth message"), __func__);
    //   break;
    default:
      // ESP_LOGD(TAG, "Bluetooth Event: %d", event);
      break;
  }
}

void setup_bluetooth(void) {
  ESP_LOGV(TAG, "Setup Bluetooth");

  if (bluetooth.begin("GerOLDin")) {
    ESP_LOGI(TAG, "Bluetooth are initted with name: GerOLDin");
  } else {
    ESP_LOGE(TAG, "Bluetooth are not initted");
    return;
  }

  if (bluetooth.setPin("0101")) {
    ESP_LOGI(TAG, "Pin are set with value 0101");
  } else {
    ESP_LOGE(TAG, "Bluetooth pin are not set");
  }

  ESP_ERROR_CHECK(bluetooth.register_callback(bluetooth_callback));
  ESP_LOGV(TAG, "Callback are registered");

  ESP_LOGI(TAG, "Bluetooth are initted");
}