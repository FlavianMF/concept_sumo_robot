#include "ir_receiver.h"

#include "esp32-hal-log.h"

static const char *TAG = "ir_receiver";

IRrecv ir_receiver(ir_receiver_pin);

decode_results ir_receiver_results;

const uint64_t PREPARE = 0x207BEF0F;
const uint64_t GO = 0xE8455D8E;
const uint64_t STOP = 0xCBB7E949;

void setup_ir_receiver() {
  ESP_LOGV(TAG, "setup_ir_receiver");

  ir_receiver.enableIRIn();
}

uint64_t read_ir() {
  if (ir_receiver.decode(&ir_receiver_results)) {
    // serialPrintUint64(ir_receiver_results.value, HEX);
    // Serial.println(ir_receiver_results.value);
    ir_receiver.resume();
    return ir_receiver_results.value;
  } else {
    return 0;
  }
}
