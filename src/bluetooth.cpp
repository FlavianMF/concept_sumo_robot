#include "bluetooth.h"

BluetoothSerial bluetooth;

static const char *TAG = "bluetooth";

void (*execute_command)(char *);

void set_execute_command(func_t new_command_function) {
  execute_command = new_command_function;
}

void bluetooth_callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
  char *data;

  switch (event) {
    case ESP_SPP_SRV_OPEN_EVT:
      ESP_LOGV(TAG, "Bluetooth are connected");
      bluetooth.println(F("Welcome to GerOLDin firmware"));
      bluetooth.println(F("Press ? to discover more informations"));
      break;
    case ESP_SPP_CLOSE_EVT:
      ESP_LOGV(TAG, "Bluetooth are disconnected");
      break;
    case ESP_SPP_DATA_IND_EVT:
      ESP_LOGD(TAG, "message: %s", (char *)param->data_ind.data);
      data = strtok((char *)param->data_ind.data, "\n");

      if (execute_command != NULL) {
        execute_command(data);
      }
      break;
    default:
      // ESP_LOGD(TAG, "Bluetooth Event: %d", event);
      break;
  }
}

bool setup_bluetooth(void) {
  ESP_LOGV(TAG, "Setup Bluetooth");

  if (bluetooth.begin("GerOLDin")) {
    ESP_LOGI(TAG, "Bluetooth are initted with name: GerOLDin");
  } else {
    ESP_LOGE(TAG, "Bluetooth are not initted");
    return false;
  }

  if (bluetooth.setPin("0101")) {
    ESP_LOGI(TAG, "Pin are set with value 0101");
  } else {
    ESP_LOGE(TAG, "Bluetooth pin are not set");
    return false;
  }

  ESP_ERROR_CHECK(bluetooth.register_callback(bluetooth_callback));
  ESP_LOGV(TAG, "Callback are registered");

  execute_command = NULL;

  ESP_LOGI(TAG, "Bluetooth are initted");
  return true;
}

void end_bluetooth() {
  bluetooth.end();
}