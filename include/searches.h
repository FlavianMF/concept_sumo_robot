#ifndef __SEARCHES_H__
#define __SEARCHES_H__

typedef void (*search_function_t)(void);

typedef enum {
  SEARCH_DIRECTION,
  SEARCH_PWM,
  SEARCH_PWM_MAX,
  SEARCH_REAR_TIME
} searches_variables_t;

extern const char *searches_commands[4];

typedef struct {
  char *name;
  search_function_t search;
  float variables[4];
} searches_t;

extern searches_t searches[];

extern const int count_searches;

extern float search_index;

extern float front_vl_max;
extern float left_vl_max;
extern float right_vl_max;
extern float back_vl_max;

extern float rear_edge_time;
extern float rear_attack_time;


#endif // __SEARCHES_H__