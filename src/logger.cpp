#include "logger.h"


void setup_logger(void)
{
  esp_log_level_set("*", ESP_LOG_VERBOSE);
}
