#include "my_eeprom.h"

#include "bluetooth.h"
#include "setup_tasks.h"
#include "tasks/bluetooth_task.h"
#include "tasks/rgb_task.h"

static const char* TAG = "eeprom";

static bool setup_flag = false;
static bool init_flag = false;

static variable_t variables[] = {
    {
        .name = (char*)"Pwm",
        .command = (char*)"pwm",
        .variable = &bluetooth_pwm,
        .init_value = 0,
    },
};

static uint16_t prepared_variable_index;

const int count_variables = sizeof(variables) / sizeof(variables[0]);

void update_prepared_variable(float value) {
  *variables[prepared_variable_index].variable = value;

  EEPROM.put(prepared_variable_index * sizeof(float), value);

  if (!EEPROM.commit()) {
    ESP_LOGE(TAG, "Error on commit variable\n");

    bluetooth.printf("Error on sabe variable\n");

    return;
  }

  ESP_LOGI(TAG, "%s saved successfully, value: %f",
           variables[prepared_variable_index].name,
           *variables[prepared_variable_index].variable);
  bluetooth.printf("\n%s saved successfully, value: %f\n",
                   variables[prepared_variable_index].name,
                   *variables[prepared_variable_index].variable);
}

bool prepare_variable_update(char* command) {
  ESP_LOGV(TAG, "Searching variable");
  for (int i = 0; i < count_variables; i++) {
    if (!strcmp(command, variables[i].command)) {
      prepared_variable_index = i;
      ESP_LOGI(TAG, "%s: %.6f", variables[i].name,
               (float)*variables[i].variable);

      bluetooth.printf("%s: %.6f\n", variables[i].name,
                       (float)*variables[i].variable);
      bluetooth.printf("New value:");

      return true;
    }
  }
  ESP_LOGW(TAG, "command \"%s\" are not a variable", command);
  return false;
}

void initialize_variables(void) {
  ESP_LOGV(TAG, "Initializing eeprom variables");

  xSemaphoreTake(setup_mutex, portMAX_DELAY);

  if (!EEPROM.begin(sizeof(float) * count_variables)) {
    ESP_LOGE(TAG, "EEPROM.begin failed");
    setup_flag = true;
    init_flag = false;
    rgb_state_t rgb_state = ANY_ERROR;
    xQueueSend(rgb_state_queue, &rgb_state, portMAX_DELAY);
    xSemaphoreGive(setup_mutex);
    return;
  }

  ESP_LOGI(TAG, "EEPROM.begin successful");
  setup_flag = true;
  init_flag = true;
  rgb_state_t rgb_state = STATUS_OK;
  xQueueSend(rgb_state_queue, &rgb_state, portMAX_DELAY);

  for (int i = 0; i < count_variables; i++) {
    EEPROM.get(i * sizeof(float), *variables[i].variable);
  }

  ESP_LOGV(TAG, "Variables are initialized");
  xSemaphoreGive(setup_mutex);
}
