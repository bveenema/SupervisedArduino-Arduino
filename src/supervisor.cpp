#include "supervisor.h"
#include "supervisor.pb.h"
#include <Arduino.h>
#include <pb_arduino.h>
#include <pb_encode.h>

supervisor::supervisor() : msg(SuperMessage_init_zero), pb_out(as_pb_ostream(Serial)) {}

void supervisor::Send() {
    pb_encode_ex(&pb_out, SuperMessage_fields, &msg, PB_ENCODE_DELIMITED);
    msg = SuperMessage_init_zero;
}