#ifndef __OUTPUTS_H__
#define __OUTPUTS_H__

typedef void (*output_function_t)(void);

typedef struct {
  float time;
  float angle;
  float linear_pwm;
  float angle_pwm;
} output_variables_t;

typedef struct {
  char *name;
  char *command;
  output_function_t output;
  output_variables_t variables;
} outputs_t;

extern char *output_variables_commands[];

extern outputs_t outputs[];

extern float output_index;

extern const int count_outputs;

// void linear_output(float time);

void debug_output_variables();


#endif // __OUTPUTS_H__