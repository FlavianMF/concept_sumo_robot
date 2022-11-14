#include "searches.h"

#include "esp32-hal-log.h"
#include "bluetooth.h"

static const char *TAG = "searches";

static void tornado_search();
static void find_edge();

const char *searches_commands[4] = {
  (char*)"sdir",
  (char*)"spwm",
  (char*)"smpwm",
  (char*)"srear",
};

searches_t searches[] = {
  {
    .name = (char*) "tornado",
    .search = tornado_search,
  },
  {
    .name = (char*) "find_edge",
    .search = find_edge,
  },
};

const int count_searches = sizeof(searches) / sizeof(searches[0]);

float search_index = 0;

void tornado_search() {
  ESP_LOGV(TAG, "tornado_search");
  bluetooth.printf("\nSEAR: tornado_search\n");
  
}

void find_edge() {
  ESP_LOGV(TAG, "find_edge");
  bluetooth.printf("\nSEAR: find_edge\n");

}