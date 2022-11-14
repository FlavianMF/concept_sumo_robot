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

#endif // __SEARCHES_H__