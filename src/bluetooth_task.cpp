#include "tasks/bluetooth_task.h"

#include "bluetooth.h"
#include "motors.h"
#include "my_eeprom.h"
#include "outputs.h"
#include "setup_tasks.h"
#include "tasks/edge_task.h"
#include "tasks/fight_task.h"
#include "tasks/motors_task.h"
#include "tasks/mpu_task.h"
#include "tasks/rgb_task.h"
#include "tasks/vl_task.h"

TaskHandle_t bluetooth_task_handle;
TaskHandle_t *debug_task_handle;

static const char *TAG = "bluetooth_task";

float bluetooth_pwm = 0;

static char *command;

typedef void (*funct_t)(void);

static funct_t debug_function;

typedef struct {
  char *name;
  char *command;
  funct_t function;
} bluetooth_functions_t;

typedef enum {
  EDGES,
  DISTANCES,
  NOTHING,
} sensors_to_debug_t;

static sensors_to_debug_t sensor_to_debug;

void debug_functions_and_variables();
void debug_bluetooth_functions();
void send_forward_move();
void prepare_debug_edges();
void prepare_debug_vls();
void debug_edge_sensors();
void debug_edge_vl_sensors();
void start_fight();

void stop_fight(char *message);

static bluetooth_functions_t bluetooth_functions[] = {
    {
        .name = (char *)"Menu",
        .command = (char *)"?",
        .function = debug_functions_and_variables,
    },
    {
        .name = (char *)"Fight",
        .command = (char *)"ft",
        .function = start_fight,
    },
    {
        .name = (char *)"Forward",
        .command = (char *)"F",
        .function = send_forward_move,
    },
    {
        .name = (char *)"Debug edge sensors",
        .command = (char *)"des",
        .function = prepare_debug_edges,
    },
    {
        .name = (char *)"Debug vl sensors",
        .command = (char *)"dvs",
        .function = prepare_debug_vls,
    },
};

static const int count_functions =
    sizeof(bluetooth_functions) / sizeof(bluetooth_functions[0]);

void stop_fight(char *message) {
  ESP_LOGV(TAG, "stop_fight");
  bluetooth.printf("\nStop fight\n");
  vTaskDelete(fight_task_handle);
  drive_motors(0, 0);
  set_execute_command(execute_bluetooth_command);

  // vTaskSuspend(vl_task_handle);
  // vTaskSuspend(mpu_task_handle);

  // vTaskResume(rgb_state_queue);
  rgb_state_t rgb_state = STATUS_OK;
  xQueueSend(rgb_state_queue, &rgb_state, portMAX_DELAY);
}

void start_fight() {
  ESP_LOGV(TAG, "start_fight");
  set_execute_command(stop_fight);
  // vTaskSuspend(motors_task_handle);
  xTaskCreatePinnedToCore(fight_task, "fight", 4096, NULL, 1,
                          &fight_task_handle, 0);
}

void stop_debug(char *message) {
  // vTaskSuspend(bluetooth_task_handle);
  // vTaskSuspend(*debug_task_handle);
  // debug_function = NULL;
  sensor_to_debug = NOTHING;
  bluetooth.printf("Stop debug function\n");
  set_execute_command(execute_bluetooth_command);
}

void prepare_debug_edges() {
  sensor_to_debug = EDGES;
  // debug_function = debug_edge_sensors;
  // debug_task_handle = &edge_task_handle;
  set_execute_command(stop_debug);
  // vTaskResume(edge_task_handle);
  // vTaskResume(bluetooth_task_handle);
}

void prepare_debug_vls() {
  sensor_to_debug = DISTANCES;
  // debug_function = debug_edge_vl_sensors;
  // debug_task_handle = &vl_task_handle;
  set_execute_command(stop_debug);
  // vTaskResume(vl_task_handle);
  // vTaskResume(bluetooth_task_handle);
}

void debug_edge_sensors() {
  ESP_LOGV(TAG, "debug_edge_sensors");

  edge_infos_t edge_infos;

  xQueueReceive(edge_queue, &edge_infos, portMAX_DELAY);

  ESP_LOGD(TAG, "LE: %d    RE: %d", edge_infos.left_edge,
           edge_infos.right_edge);
  bluetooth.printf("LE: %d    RE: %d\n", edge_infos.left_edge,
                   edge_infos.right_edge);

  vTaskDelay(pdMS_TO_TICKS(5));
}

void debug_edge_vl_sensors() {
  ESP_LOGV(TAG, "debug_vl_sensors");

  vl_readings_t vl_readings;

  xQueueReceive(vl_queue, &vl_readings, portMAX_DELAY);

  ESP_LOGD(TAG, "F: %d", vl_readings.front_reading);
  bluetooth.printf("F: %d\n", vl_readings.front_reading);

  // vTaskDelay(pdMS_TO_TICKS(3));
}

void send_forward_move() {
  ESP_LOGV(TAG, "send_forward_move");
  motors_functions_t function = FORWARD;
  xQueueSend(motors_queue, &function, portMAX_DELAY);
}

void debug_functions_and_variables() {
  ESP_LOGV(TAG, "debug_functions_and_variables");
  debug_bluetooth_functions();
  debug_eeprom_variables();
  debug_output_variables();
  debug_searches_variables();
}

void debug_bluetooth_functions() {
  for (int i = 0; i < count_functions; i++) {
    ESP_LOGD(TAG, "[%s] %s", bluetooth_functions[i].command,
             bluetooth_functions[i].name);
    bluetooth.printf("[%s] %s\n", bluetooth_functions[i].command,
                     bluetooth_functions[i].name);
  }
}

void read_float_value(char *message) {
  ESP_LOGD(TAG, "value received: %s", message);

  if (!strcmp(message, "?")) {
    ESP_LOGV(TAG, "cancelling operation");

    bluetooth.printf("End operation\n");

    set_execute_command(execute_bluetooth_command);
    return;
  }

  float variable = (float)atof(message);

  update_prepared_variable(variable);
  set_execute_command(execute_bluetooth_command);
}

void execute_bluetooth_command(char *message) {
  ESP_LOGD(TAG, "command received: %s", message);

  for (int i = 0; i < count_functions; i++) {
    if (!strcmp(message, bluetooth_functions[i].command)) {
      bluetooth_functions[i].function();
      return;
    }
  }

  if (prepare_variable_update(message)) {
    set_execute_command(read_float_value);
    return;
  }

  // if (prepare_output_variable_update(message))  {
  //   set
  // }
}

void bluetooth_task(void *pvParameters) {
  ESP_LOGV(TAG, "bluetooth task init");

  xSemaphoreTake(setup_mutex, portMAX_DELAY);
  if (!setup_bluetooth()) {
    rgb_state_t rgb_state = ANY_ERROR;
    xQueueSend(rgb_state_queue, &rgb_state, portMAX_DELAY);
  } else {
    rgb_state_t rgb_state = STATUS_OK;
    xQueueSend(rgb_state_queue, &rgb_state, portMAX_DELAY);
  }
  xSemaphoreGive(setup_mutex);

  set_execute_command(execute_bluetooth_command);

  debug_function = NULL;
  sensor_to_debug = NOTHING;
  // vTaskSuspend(NULL);
  while (true) {
    switch (sensor_to_debug) {
      case EDGES:
      bluetooth.printf("Debug edges:\n");
        while (sensor_to_debug == EDGES) {
          debug_edge_sensors();
        }
        break;
      case DISTANCES:
      bluetooth.printf("Debug distances:\n");
        while (sensor_to_debug == DISTANCES) {
          debug_edge_vl_sensors();
        }
        break;
      default:
        break;
    }
    // debug_function();
    vTaskDelay(pdMS_TO_TICKS(5));
  }
}
