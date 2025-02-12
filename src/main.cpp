#include <Arduino.h>
#include "pb_encode.h"
#include "pb_decode.h"

#include "supervisor.h"
#include "superlog.h"
#include "button.h"

#define START_BUTTON_PIN IO0
#define STOP_BUTTON_PIN IO1

Button StartButton(START_BUTTON_PIN, Supervisor.outMsg.has_StartButton, Supervisor.outMsg.StartButton);
Button StopButton(STOP_BUTTON_PIN, Supervisor.outMsg.has_StopButton, Supervisor.outMsg.StopButton);


void setup() {
  // Initialize the serial port
  Serial.begin(115200);

  // Initialize the IO pins
  pinMode(IO2, OUTPUT);

  // Initialize the logger
  SuperLog.setLevel(superLog::Level::INFO);

  // Initialize the buttons
  StartButton.init();
  StopButton.init();
}

void loop() {
  // Print millis every second
  static uint32_t printTimer = 0;
  if(millis() - printTimer >= 1000) {
    printTimer = millis();

    // Toggle the IO pin to prove the program isn't stuck
    static bool p_IO2 = false;
    digitalWrite(IO2, p_IO2);
    p_IO2 = !p_IO2;

    // log the current time
    SuperLog.log(superLog::Level::INFO, "Millis: %lu", millis());

    // // Log the IO state
    SuperLog.log(superLog::Level::INFO, "IO2: %d", digitalRead(IO2));
  }

  // Handle all communication (both sending and receiving)
  Supervisor.update();

  // Update the buttons
  StartButton.update();
  StopButton.update();
}