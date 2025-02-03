#include "supervisor.h"
#include "supervisor.pb.h"
#include <Arduino.h>
#include <pb_arduino.h>
#include <pb_encode.h>

supervisor::supervisor() : msg(SuperMessage_init_zero), pb_out(as_pb_ostream(Serial)) {}


void supervisor::send() {
    // Sizing the message when the size is 0 takes ~10us, 67us when the size is 22.
    // Encoding/Sending a 22 byte message takes ~264us
    // This was testing performed on ClearCore with a 115200 baud rate
    // 10us penalty to check size and prevent empty sends is worth the cost but calls to supervisor::send()
    // should be considered carefully in time critical applications.
    size_t msg_size = 0;
    pb_get_encoded_size(&msg_size, SuperMessage_fields, &msg);
    if (msg_size > 0) {
        pb_encode_ex(&pb_out, SuperMessage_fields, &msg, PB_ENCODE_DELIMITED);
    }
    msg = SuperMessage_init_zero;
}

///--- Instrumented version of the send function for timing analysis
// #include "superlog.h"
// bool encode_string(pb_ostream_t *stream, const pb_field_t *field, void * const *arg) {
//     const char *str = (const char*)(*arg);
//     if (!pb_encode_tag_for_field(stream, field)) {
//         return false;
//     }
//     return pb_encode_string(stream, (const pb_byte_t*)str, strlen(str));
// }

// void supervisor::send() {
//     uint32_t startTime = micros();
//         size_t msg_size = 0;
//         pb_get_encoded_size(&msg_size, SuperMessage_fields, &msg);
//     uint32_t getSizeTime = micros() - startTime;
//         if (msg_size > 0) {
//             pb_encode_ex(&pb_out, SuperMessage_fields, &msg, PB_ENCODE_DELIMITED);
//         }
//     uint32_t sendTime = micros() - startTime - getSizeTime;
//     uint32_t totalTime = micros() - startTime;

//     char buffer[256];
//     snprintf(buffer, sizeof(buffer), "Message size: %d, Get size time: %d, Send time: %d, Total time: %d", msg_size, getSizeTime, sendTime, totalTime);
    
//     SuperLogMessage logger = SuperLogMessage_init_zero;
//     logger.level = SuperLogLevel_debug;
//     logger.message.funcs.encode = &encode_string;
//     logger.message.arg = buffer;

//     msg.has_log = true;
//     msg.log = logger;
//     pb_encode_ex(&pb_out, SuperMessage_fields, &msg, PB_ENCODE_DELIMITED);

//     msg = SuperMessage_init_zero;
// }
///--- End instrumented version