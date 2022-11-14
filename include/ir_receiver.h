#ifndef __IR_RECEIVER_H__
#define __IR_RECEIVER_H__
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRutils.h>

#define ir_receiver_pin 25

extern IRrecv ir_receiver;

extern decode_results ir_receiver_results;

extern const uint64_t PREPARE;
extern const uint64_t GO;
extern const uint64_t STOP;

void setup_ir_receiver();

uint64_t read_ir();
#endif // __IR_RECEIVER_H__