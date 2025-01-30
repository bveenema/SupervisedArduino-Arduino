#include <Arduino.h>
#include <pb_arduino.h>
#include "pb_encode.h"
#include "pb_decode.h"
#include <stdio.h>

#include "test.pb.h"
#include "namespace.pb.h"

// pb_istream_s pb_in = as_pb_istream(Serial);
pb_ostream_s pb_out = as_pb_ostream(Serial);
//and then use pb_encode(&pb_out, ...) and pb_decode(&pb_in).


NameSpace ns = NameSpace_init_zero;
TestMessage test_message = TestMessage_init_zero;
pbm_IOState io_state = pbm_IOState_init_zero;

void setup() {
  // Initialize the serial port
  Serial.begin(115200);

  pinMode(IO2, OUTPUT);

}

void loop() {
  // Print millis every second
  static uint32_t printTimer = 0;
  if(millis() - printTimer >= 1000) {
    printTimer = millis();

    // Toggle the IO pin to prove the program isn't stuck
    digitalWrite(IO2, io_state.pIO2);
    io_state.pIO2 = !io_state.pIO2;
    io_state.pIO0 = 0;
    io_state.pIO1 = 0;

    ns.which_child = NameSpace_io_state_tag;
    ns.child.io_state = io_state;
    pb_encode_ex(&pb_out, NameSpace_fields, &ns, PB_ENCODE_DELIMITED);
    

    // Print the millis using nanopb on the serial port
    test_message.number = millis();
    ns.which_child = NameSpace_test_message_tag;
    ns.child.test_message = test_message;
    pb_encode_ex(&pb_out, NameSpace_fields, &ns, PB_ENCODE_DELIMITED);
  }
}