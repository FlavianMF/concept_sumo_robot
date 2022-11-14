#include "searches.h"

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
    .search = tornado_search,
  },
};

const int count_searches = sizeof(searches) / sizeof(searches[0]);

float search_index = 0;

void tornado_search() {

}

void find_edge() {

}