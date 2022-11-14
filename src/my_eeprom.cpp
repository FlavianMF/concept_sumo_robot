#include "my_eeprom.h"

#include "bluetooth.h"
#include "motors.h"
#include "move_functions.h"
#include "outputs.h"
#include "searches.h"
#include "setup_tasks.h"
#include "tasks/bluetooth_task.h"
#include "tasks/edge_task.h"
#include "tasks/motors_task.h"
#include "tasks/rgb_task.h"

static const char* TAG = "eeprom";

static bool setup_flag = false;
static bool init_flag = false;

enum output_variable_flag_t {
  TIME,
  ANGLE,
  LPWM,
  APWM,
};

static output_variable_flag_t output_variable_flag;

enum variable_type_flag_t {
  VARIABLES,
  OUTPUTS,
  SEARCHES,
};

static variable_type_flag_t variable_type_flag;

static searches_variables_t search_variable_flag;

void debug_output_variables();

static variable_t variables[] = {
    {
        .name = (char*)"Pwm",
        .command = (char*)"pwm",
        .variable = &pwm,
        .init_value = 0,
    },
    {
        .name = (char*)"Time",
        .command = (char*)"time",
        .variable = &linear_time,
        .init_value = 0,
    },
    {
        .name = (char*)"Accel time",
        .command = (char*)"acl",
        .variable = &accel_time,
        .init_value = 100,
    },
    {
        .name = (char*)"Slow time",
        .command = (char*)"slw",
        .variable = &slow_time,
        .init_value = 100,
    },
    {
        .name = (char*)"Left Gain",
        .command = (char*)"lg",
        .variable = &left_gain,
        .init_value = 1,
    },
    {
        .name = (char*)"Right Gain",
        .command = (char*)"rg",
        .variable = &right_gain,
        .init_value = 1,
    },
    {
        .name = (char*)"Linear KP",
        .command = (char*)"lkp",
        .variable = &linear_kp,
        .init_value = 1,
    },
    {
        .name = (char*)"Linear KI",
        .command = (char*)"lki",
        .variable = &linear_ki,
        .init_value = 0,
    },
    {
        .name = (char*)"Linear KD",
        .command = (char*)"lkd",
        .variable = &linear_kd,
        .init_value = 0,
    },
    {
        .name = (char*)"Right Edge Limit",
        .command = (char*)"rel",
        .variable = &right_edge_limit,
        .init_value = 2048,
    },
    {
        .name = (char*)"Left Edge Limit",
        .command = (char*)"lel",
        .variable = &right_edge_limit,
        .init_value = 2048,
    },
    {
        .name = (char*)"Current output",
        .command = (char*)"co",
        .variable = &output_index,
        .init_value = 0,
    },
    {
        .name = (char*)"Current searches",
        .command = (char*)"cs",
        .variable = &search_index,
        .init_value = 0,
    },
};

static uint16_t prepared_variable_index;

static int count_variables = sizeof(variables) / sizeof(variables[0]);

void debug_output_variables() {
  // vTaskDelay(pdMS_TO_TICKS(500));
  bluetooth.printf("\n%s\n", outputs[(int)output_index].name);
  bluetooth.printf("time: %f\n", outputs[(int)output_index].variables.time);
  bluetooth.printf("angle: %f\n", outputs[(int)output_index].variables.angle);
  bluetooth.printf("l_pwm: %f\n",
                   outputs[(int)output_index].variables.linear_pwm);
  bluetooth.printf("a_pwm: %f\n",
                   outputs[(int)output_index].variables.angle_pwm);
  // bluetooth.printf(
  //     "\n %s\ntime: %f\nangle: %f\nl_pwm: %f\na_pwm: %f\n",
  //     outputs[(int)*variables[prepared_variable_index].variable].name,
  //     outputs[(int)*variables[prepared_variable_index].variable].variables.time,
  //     outputs[(int)*variables[prepared_variable_index].variable]
  //         .variables.angle,
  //     outputs[(int)*variables[prepared_variable_index].variable]
  //         .variables.linear_pwm,
  //     outputs[(int)*variables[prepared_variable_index].variable]
  //         .variables.angle_pwm);
}

void debug_searches_variables() {
  bluetooth.printf("\n%s\n", searches[(int)search_index].name);
  for (int i = 0; i < 4; i++) {
    bluetooth.printf("%s: %f\n", searches_commands[i],
                     searches[(int)search_index].variables[i]);
  }
}

void debug_outputs() {
  // vTaskDelay(pdMS_TO_TICKS(1));
  bluetooth.printf("\n");
  for (int i = 0; i < count_outputs; i++) {
    bluetooth.printf("[%i] %s\n", i, outputs[i].name);
  }
}

void debug_searches() {
  // vTaskDelay(pdMS_TO_TICKS(1));
  bluetooth.printf("\n");
  for (int i = 0; i < count_searches; i++) {
    bluetooth.printf("[%i] %s\n", i, searches[i].name);
  }
}

void debug_eeprom_variables() {
  bluetooth.printf("\n");
  for (int i = 0; i < count_variables; i++) {
    ESP_LOGD(TAG, "[%s] %s: %.6f", variables[i].command, variables[i].name,
             *variables[i].variable);
    bluetooth.printf("[%s] %s: %.6f\n", variables[i].command, variables[i].name,
                     *variables[i].variable);
    // vTaskDelay(pdMS_TO_TICKS(10));
    // bluetooth.
  }

  // debug_output_variables();
}

void update_prepared_variable(float value) {
  switch (variable_type_flag) {
    case VARIABLES:
      *variables[prepared_variable_index].variable = value;
      break;
    case OUTPUTS:
      switch (output_variable_flag) {
        case TIME:
          outputs[(int)output_index].variables.time = value;
          break;
        case ANGLE:
          outputs[(int)output_index].variables.angle = value;
          break;
        case LPWM:
          outputs[(int)output_index].variables.linear_pwm = value;
          break;
        case APWM:
          outputs[(int)output_index].variables.angle_pwm = value;
          break;
        default:
          return;
          break;
      }
      break;
    case SEARCHES:
      searches[(int)search_index].variables[search_variable_flag] = value;
      break;
    default:
      return;
      break;
  }

  EEPROM.put(prepared_variable_index * sizeof(float), value);

  if (!EEPROM.commit()) {
    ESP_LOGE(TAG, "Error on commit variable\n");

    bluetooth.printf("Error on save variable\n");

    return;
  }

  switch (variable_type_flag) {
    case VARIABLES:
      ESP_LOGI(TAG, "%s saved successfully, value: %f",
               variables[prepared_variable_index].name,
               *variables[prepared_variable_index].variable);
      bluetooth.printf("\n%s saved successfully, value: %f\n",
                       variables[prepared_variable_index].name,
                       *variables[prepared_variable_index].variable);
      break;
    case OUTPUTS:
      switch (output_variable_flag) {
        case TIME:
          ESP_LOGI(TAG, "time saved successfully, value: %f",
                   outputs[(int)output_index].variables.time);
          bluetooth.printf("time saved successfully, value: %f\n",
                           outputs[(int)output_index].variables.time);
          break;
        case ANGLE:
          ESP_LOGI(TAG, "angle saved successfully, value: %f",
                   outputs[(int)output_index].variables.angle);
          bluetooth.printf("angle saved successfully, value: %f\n",
                           outputs[(int)output_index].variables.angle);
          break;
        case LPWM:
          ESP_LOGI(TAG, "l_pwm saved successfully, value: %f",
                   outputs[(int)output_index].variables.linear_pwm);
          bluetooth.printf("l_pwm saved successfully, value: %f\n",
                           outputs[(int)output_index].variables.linear_pwm);
          break;
        case APWM:
          ESP_LOGI(TAG, "a_pwm saved successfully, value: %f",
                   outputs[(int)output_index].variables.angle_pwm);
          bluetooth.printf("a_pwm saved successfully, value: %f\n",
                           outputs[(int)output_index].variables.angle_pwm);
          break;
        default:
          return;
          break;
      }
      break;
    case SEARCHES:
      bluetooth.printf("%s saved, value: %f\n", searches_commands[search_variable_flag], searches[(int)search_index].variables[search_variable_flag]);
      break;
    default:
      break;
  }

  if (!strcmp(variables[prepared_variable_index].command, "co")) {
    debug_output_variables();
  } else if (!strcmp(variables[prepared_variable_index].command, "cs")) {
    debug_searches_variables();
  }
}

bool prepare_variable_update(char* command) {
  ESP_LOGV(TAG, "Searching variable");

  for (int i = 0; i < count_variables; i++) {
    if (!strcmp(command, variables[i].command)) {
      variable_type_flag = VARIABLES;
      prepared_variable_index = i;
      ESP_LOGI(TAG, "%s: %.6f", variables[i].name,
               (float)*variables[i].variable);

      if (!strcmp(command, "co")) {
        debug_outputs();
        bluetooth.printf("\n");
      } else if (!strcmp(command, "cs")) {
        debug_searches();
        bluetooth.printf("\n");
      }

      bluetooth.printf("%s: %.6f\n", variables[i].name,
                       (float)*variables[i].variable);
      bluetooth.printf("New value:");

      return true;
    }
  }

  if (!strcmp(command, "otime")) {
    variable_type_flag = OUTPUTS;
    output_variable_flag = TIME;
    prepared_variable_index = (count_variables) + ((int)output_index * 4) + 1;
    bluetooth.printf("\n%s\ntime: %f\n New: ", outputs[(int)output_index].name,
                     outputs[(int)output_index].variables.time);
    return true;
  } else if (!strcmp(command, "oangle")) {
    variable_type_flag = OUTPUTS;
    output_variable_flag = ANGLE;
    prepared_variable_index = (count_variables) + ((int)output_index * 4) + 2;
    bluetooth.printf("\n%s\nangle: %f\n New: ", outputs[(int)output_index].name,
                     outputs[(int)output_index].variables.angle);
    return true;
  } else if (!strcmp(command, "lpwm")) {
    variable_type_flag = OUTPUTS;
    output_variable_flag = LPWM;
    prepared_variable_index = (count_variables) + ((int)output_index * 4) + 3;
    bluetooth.printf("\n%s\nlpwm: %f\n New: ", outputs[(int)output_index].name,
                     outputs[(int)output_index].variables.linear_pwm);
    return true;
  } else if (!strcmp(command, "apwm")) {
    variable_type_flag = OUTPUTS;
    output_variable_flag = APWM;
    prepared_variable_index = (count_variables) + ((int)output_index * 4) + 4;
    bluetooth.printf("\n%s\napwm: %f\n New: ", outputs[(int)output_index].name,
                     outputs[(int)output_index].variables.angle_pwm);
    return true;
  }

  for (int i = 0; i < 4; i++) {
    if (!strcmp(command, searches_commands[i])) {
      variable_type_flag = SEARCHES;
      search_variable_flag = (searches_variables_t)i;
      prepared_variable_index = ((count_outputs * 4) + count_variables) +
                                ((int)search_index * 4) + (i + 1);
      bluetooth.printf("\n%s\n%s: %f\n New: ", searches[(int)search_index].name,
                       searches_commands[i],
                       searches[(int)search_index].variables[i]);
      return true;
    }
  }

  ESP_LOGW(TAG, "command \"%s\" are not a variable", command);
  return false;
}

void initialize_variables(void) {
  ESP_LOGV(TAG, "Initializing eeprom variables");

  xSemaphoreTake(setup_mutex, portMAX_DELAY);

  if (!EEPROM.begin(sizeof(float) * (count_variables + (count_outputs * 4) +
                                     (count_searches * 4)))) {
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

  int i = 0;
  for (; i < count_variables; i++) {
    EEPROM.get(i * sizeof(float), *variables[i].variable);
  }

  for (int j = 0; j < count_outputs; j++) {
    EEPROM.get(++i * sizeof(float), outputs[j].variables.time);
    EEPROM.get(++i * sizeof(float), outputs[j].variables.angle);
    EEPROM.get(++i * sizeof(float), outputs[j].variables.linear_pwm);
    EEPROM.get(++i * sizeof(float), outputs[j].variables.angle_pwm);
  }

  for (int j = 0; j < count_searches; j++) {
    for (int k = 0; k < 4; k++) {
      EEPROM.get(++i * sizeof(float), searches[j].variables[k]);
    }
  }

  ESP_LOGV(TAG, "Variables are initialized");
  xSemaphoreGive(setup_mutex);
}
