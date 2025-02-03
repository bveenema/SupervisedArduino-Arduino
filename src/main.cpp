#include <Arduino.h>
#include "pb_encode.h"
#include "pb_decode.h"

#include "pb_main.h"
#include "logger.h"

#include "io_state.pb.h"
IOState io_state = IOState_init_zero;

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
    io_state.p_IO2 = !io_state.p_IO2;
    digitalWrite(IO2, io_state.p_IO2);


    // log the current time
    Logger.Log(ProtoLog::Level::INFO, "Millis: %lu", millis());

    // Log the IO state
    Supervisor.msg.has_io_state = true;
    Supervisor.msg.io_state = io_state;

    // pb_send();
    Supervisor.Send();
  }
}