#include "button.h"
#include "Arduino.h"

Button::Button(uint8_t pin, bool& sm_has, ButtonMessage& sm_field) : pin(pin), sm_has(sm_has), sm_field(sm_field) {
    Supervisor.addComponent(this);
}

void Button::init() {
    pinMode(pin, INPUT_PULLUP);
    lastPressed = false;
    pressed = false;
    Supervisor.readyToPublish(this);
}

bool Button::onPress(PressCallback callback) {
    if (callbackCount >= MAX_CALLBACKS) {
        return false;
    }
    callbacks[callbackCount++] = callback;
    return true;
}

void Button::update() {
    // Read button state (inverted because of INPUT_PULLUP)
    bool currentState = !digitalRead(pin);
    
    // If state changed
    if (currentState != pressed) {
        pressed = currentState;
        
        // If button was just pressed, call all callbacks and notify supervisor
        if (pressed && !lastPressed) {
            for (size_t i = 0; i < callbackCount; i++) {
                if (callbacks[i]) {
                    callbacks[i]();
                }
            }
        }
        Supervisor.readyToPublish(this);
    }
    
    lastPressed = pressed;
}

bool Button::publish(SuperMessage& msg) {
    sm_has = true;
    sm_field.state = pressed;
    return true;
}