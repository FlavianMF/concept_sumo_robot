#ifndef __MY_EEPROM_H__
#define __MY_EEPROM_H__
#include <EEPROM.h>

typedef struct {
  char *name;
  char *command;
  float *variable;
  float init_value;
} variable_t;

typedef enum {
  INITIALIZE,
  UPDATE,
  RESET,
  DEBUG,
} eeprom_operations_t;

void operate_variable(eeprom_operations_t operation, char *command);


void initialize_variables(void);

bool prepare_variable_update(char* command);

void update_prepared_variable(float value );

void debug_eeprom_variables();

void debug_searches();

void debug_searches_variables();
// extern variable_t variables[];

#endif // __MY_EEPROM_H__