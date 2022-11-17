#include "searches.h"

#include "bluetooth.h"
#include "esp32-hal-log.h"
#include "motors.h"
#include "tasks/vl_task.h"
#include "tasks/edge_task.h"

static const char *TAG = "searches";

static void tornado_search();
static void find_edge();

float front_vl_max = 500;
float left_vl_max = 500;
float right_vl_max = 500;
float back_vl_max = 500;

float rear_edge_time = 500;
float rear_attack_time = 500;

const char *searches_commands[4] = {
    (char *)"sdir",
    (char *)"spwm",
    (char *)"smpwm",
    (char *)"srear",
};

searches_t searches[] = {
    {
        .name = (char *)"tornado",
        .search = tornado_search,
    },
    {
        .name = (char *)"find_edge",
        .search = find_edge,
    },
};

const int count_searches = sizeof(searches) / sizeof(searches[0]);

float search_index = 0;

typedef enum {
  SEARCHING,
  ATTACKING,
  GO_BACK_EDGE,
  GO_BACK_ATTACK,
} tornado_states_t;

void tornado_search() {
  ESP_LOGV(TAG, "tornado_search");
  bluetooth.printf("\nSEAR: tornado_search\n");

  float *search_pwm = &searches[(int)search_index].variables[SEARCH_PWM];
  float *attacking_pwm = &searches[(int)search_index].variables[SEARCH_PWM_MAX];
  float direction = searches[(int)search_index].variables[SEARCH_DIRECTION];
  float *attacking_time = &searches[(int)search_index].variables[SEARCH_REAR_TIME];

  vl_readings_t vl_readings;
  edge_infos_t edge_infos;

  tornado_states_t state = SEARCHING;
  bool receive_info = false;
  uint64_t start_attack = 0;

  while (true) {
    switch (state) {
      case SEARCHING:
        bluetooth.printf("Searching: %s\n", direction == 1 ? "right" : "left");
        drive_motors(*search_pwm * (direction),
                     *search_pwm * (direction * -1));
        direction *= -1;
        break;
      case ATTACKING:
        bluetooth.printf("Attacking\n");
        // start_attack = millis();
        drive_motors(*attacking_pwm, *attacking_pwm);
        break;
      case GO_BACK_EDGE:
        bluetooth.printf("go_back_edge\n");
        drive_motors(-*search_pwm, -*search_pwm);
        vTaskDelay(pdMS_TO_TICKS(rear_edge_time));
        break;
      case GO_BACK_ATTACK:
        bluetooth.printf("go_back_attack\n");
        drive_motors(-*search_pwm, -*search_pwm);
        vTaskDelay(pdMS_TO_TICKS(rear_attack_time));
        break;
      default:
        break;
    }

    while (!receive_info) {
      if (state == ATTACKING && millis() > start_attack + *attacking_time) {
        state = GO_BACK_ATTACK;
        drive_motors(0,0);

        receive_info = true;
        break;
      }
      if (xQueueReceive(edge_queue, &edge_infos, 1) == pdTRUE) {
        if (edge_infos.left_edge == 1 || edge_infos.right_edge == 1) {
          state = GO_BACK_EDGE;
          drive_motors(0,0);


          receive_info = true;
          break;
        }
      }
      if (xQueueReceive(vl_queue, &vl_readings, 1) == pdTRUE) {
          // bluetooth.printf("front vl: %d\n", vl_readings.front_reading);
        if (vl_readings.front_reading < front_vl_max) {
          // bluetooth.printf("front vl: %d\n", vl_readings.front_reading);
          if (state != ATTACKING) {
            // bluetooth.printf("Attacking\n");
            drive_motors(0,0);
            start_attack = millis();
          }

          state = ATTACKING;

          receive_info = true;
          break;
        } else if (state != SEARCHING) {
          state = SEARCHING;


          receive_info = true;
          break;
        }
      }
      vTaskDelay(1);
    }
    receive_info = false;
    vTaskDelay(1);
  }
}

void find_edge() {
  ESP_LOGV(TAG, "find_edge");
  bluetooth.printf("\nSEAR: find_edge\n");
}