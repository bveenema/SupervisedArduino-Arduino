#include "supervisor.h"
#include "supervisor.pb.h"
#include <Arduino.h>
#include <pb_arduino.h>
#include <pb_encode.h>
#include <pb_decode.h>

supervisor::supervisor() : pb_out(as_pb_ostream(Serial)) {}

void supervisor::addComponent(ComponentTemplate* component) {
    if (componentCount >= MAX_COMPONENTS) {
        // Handle error - too many components
        return;
    }

    // Set the component ID
    component->setComponentId(componentCount++);
    
    // Add to linked list
    if (head == nullptr) {
        head = component;
    } else {
        // Add to end of list
        ComponentTemplate* current = head;
        while (current->getNext() != nullptr) {
            current = current->getNext();
        }
        current->setNext(component);
    }
    component->setNext(nullptr);
}

void supervisor::readyToPublish(ComponentTemplate* component) {
    pendingUpdates |= (1ULL << component->getComponentId());
}

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

void supervisor::update() {
    // First handle sending any pending updates
    if (pendingUpdates != 0) {  // If any bits are set
        resetSuperMessage(outMsg);
        
        // Only update from components that have changes
        ComponentTemplate* current = head;
        while (current != nullptr) {
            size_t id = current->getComponentId();
            if (pendingUpdates & (1ULL << id)) {
                current->publish(outMsg);
            }
            current = current->getNext();
        }
        
        pb_encode_ex(&pb_out, SuperMessage_fields, &outMsg, PB_ENCODE_DELIMITED);
        pendingUpdates = 0;  // Clear all bits
    }

    // Then handle receiving any incoming messages
    static uint8_t buffer[256];
    static size_t buffer_index = 0;
    static size_t message_size = 0;
    static bool size_received = false;

    while (Serial.available() > 0) {
        uint8_t byte = Serial.read();
        buffer[buffer_index++] = byte;

        if (!size_received) {
            size_t varint_size = 0;
            if (decode_varint(buffer, buffer_index, varint_size, message_size)) {
                size_received = true;
                buffer_index -= varint_size;
                memmove(buffer, buffer + varint_size, buffer_index);
            }
        }

        if (size_received && buffer_index >= message_size) {
            // Convert buffer to stream
            pb_istream_t stream = pb_istream_from_buffer(buffer, message_size);
            
            // Initialize inMsg
            resetSuperMessage(inMsg);
            
            // Set up decode callbacks for all components
            ComponentTemplate* current = head;
            while (current != nullptr) {
                current->preReceive(inMsg);
                current = current->getNext();
            }
            
            // Decode the message
            if (pb_decode(&stream, SuperMessage_fields, &inMsg)) {
                // Notify all components about the new message
                current = head;
                while (current != nullptr) {
                    current->receive(inMsg);
                    current = current->getNext();
                }
            }

            // Reset for next message
            buffer_index = 0;
            size_received = false;
            message_size = 0;
        }
    }
}

void supervisor::resetSuperMessage(SuperMessage& msg) {
    memset(&msg, 0, sizeof(SuperMessage));
}

///--- Instrumented version of the send function for timing analysis
// #include "superlog.h:
// bool encodeString(pb_ostream_t *stream, const pb_field_t *field, void * const *arg) {
//     const char *str = (const char*)(*arg);
//     if (!pb_encode_tag_for_field(stream, field)) {
//         return false;
//     }
//     return pb_encodeString(stream, (const pb_byte_t*)str, strlen(str));
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
//         logger.message.funcs.encode = &encodeString;
//         logger.message.arg = buffer;

//         msg.has_log = true;
//         msg.log = logger;
//         pb_encode_ex(&pb_out, SuperMessage_fields, &msg, PB_ENCODE_DELIMITED);
//     }

//     msg = SuperMessage_init_zero;
// }
///--- End instrumented version