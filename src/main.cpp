#include <Arduino.h>
#include "pb_encode.h"
#include "pb_decode.h"

#include "supervisor.h"
#include "superlog.h"

#include "io_state.pb.h"
IOState io_state = IOState_init_zero;

void setup() {
  // Initialize the serial port
  Serial.begin(115200);

  // Initialize the IO pins
  pinMode(IO2, OUTPUT);

  // Initialize the logger
  SuperLog.setLevel(superLog::Level::INFO);
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
    // SuperLog.log(superLog::Level::INFO, "Millis: %lu", millis());

    // // Log the IO state
    // Supervisor.msg.has_io_state = true;
    // Supervisor.msg.io_state = io_state;
  }

  static uint32_t sendTimer = 0;
  if(millis() - sendTimer >= 100) {
    sendTimer = millis();
    Supervisor.send();
  }

  // Read the serial port
  Supervisor.update();
}