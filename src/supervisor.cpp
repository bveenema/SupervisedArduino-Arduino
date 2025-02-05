#include "supervisor.h"
#include "supervisor.pb.h"
#include <Arduino.h>
#include <pb_arduino.h>
#include <pb_encode.h>
#include <pb_decode.h>

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

#include <stdint.h>
#include <stddef.h>

bool decode_varint(const uint8_t *buffer, size_t buffer_size, size_t &varint_size, size_t &value) {
    value = 0;
    varint_size = 0;

    for (size_t i = 0; i < buffer_size; i++) {
        uint8_t byte = buffer[i];
        value |= (byte & 0x7F) << (7 * i);
        varint_size++;

        if (!(byte & 0x80)) {
            return true;
        }
    }

    return false; // Buffer ended before varint was fully decoded
}


bool print_string(pb_istream_t *stream, const pb_field_t *field, void **arg) {
    uint8_t buffer[1024] = {0};
    
    /* We could read block-by-block to avoid the large buffer... */
    if (stream->bytes_left > sizeof(buffer) - 1)
        return false;
    
    if (!pb_read(stream, buffer, stream->bytes_left))
        return false;
    
    /* Print the string, in format comparable with protoc --decode.
     * Format comes from the arg defined in main().
     */
    printf((char*)*arg, buffer);
    return true;
}

void supervisor::update() {
    static char printBuffer[256];
    static uint8_t buffer[256];
    static size_t buffer_index = 0;
    static size_t message_size = 0;
    static bool size_received = false;

    while (Serial.available() > 0) {
        uint8_t byte = Serial.read();
        buffer[buffer_index++] = byte;
        // snprintf(printBuffer, sizeof(printBuffer), "Received byte: 0x%02X", byte);
        // Serial.println(printBuffer);

        if (!size_received) {
            size_t varint_size = 0;
            if (decode_varint(buffer, buffer_index, varint_size, message_size)) {
                size_received = true;
                buffer_index -= varint_size;
                snprintf(printBuffer, sizeof(printBuffer), "Received message size: %d", message_size);
                Serial.println(printBuffer);
                memmove(buffer, buffer + varint_size, buffer_index);
            }
        }

        if (size_received && buffer_index >= message_size) {
            for (size_t i = 0; i < message_size; i++) {
                snprintf(printBuffer + i * 2, sizeof(printBuffer) - i * 2, "%02X", buffer[i]);
            }
            Serial.println(printBuffer);
            // Decode the message
            pb_istream_t stream = pb_istream_from_buffer(buffer, message_size);
            SuperMessage msg2 = SuperMessage_init_zero;
            msg2.log.message.funcs.decode = &print_string;
            msg2.log.message.arg = (void*)"%s";
            if (pb_decode(&stream, SuperMessage_fields, &msg2)) {
                // echo the message
                
                snprintf(printBuffer, sizeof(printBuffer), "Received message: Level-%d, Message-%s", msg2.log.level, msg2.log.message);
                Serial.println(printBuffer);
            }

            // Reset for the next message
            buffer_index = 0;
            size_received = false;
            message_size = 0;
        }
    }
}

///--- Instrumented version of the send function for timing analysis
// #include "superlog.h:
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

//     if (msg_size > 0) {
//         char buffer[256];
//         snprintf(buffer, sizeof(buffer), "Message size: %d, Get size time: %d, Send time: %d, Total time: %d", msg_size, getSizeTime, sendTime, totalTime);
        
//         SuperLogMessage logger = SuperLogMessage_init_zero;
//         logger.level = SuperLogLevel_debug;
//         logger.message.funcs.encode = &encode_string;
//         logger.message.arg = buffer;

//         msg.has_log = true;
//         msg.log = logger;
//         pb_encode_ex(&pb_out, SuperMessage_fields, &msg, PB_ENCODE_DELIMITED);
//     }

//     msg = SuperMessage_init_zero;
// }
///--- End instrumented version