#include "pb_main.h"
#include "main.pb.h"
#include <Arduino.h>
#include <pb_arduino.h>
#include <pb_encode.h>

supervisor Supervisor;

supervisor::supervisor() : msg(MainMessage_init_zero), pb_out(as_pb_ostream(Serial)) {}

void supervisor::Send() {
    pb_encode_ex(&pb_out, MainMessage_fields, &msg, PB_ENCODE_DELIMITED);
    msg = MainMessage_init_zero;
}