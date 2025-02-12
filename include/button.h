#ifndef __BUTTON__
#define __BUTTON__

#include "component_template.h"
#include "button.pb.h"

class Button : public ComponentTemplate {
public:
    // Function pointer type for callbacks
    typedef void (*PressCallback)();
    static constexpr size_t MAX_CALLBACKS = 4;  // Maximum number of callbacks
    
    Button(uint8_t pin, bool& sm_has, ButtonMessage& sm_field);
    
    void init();
    void update();
    
    // Add a callback to be called when button is pressed
    bool onPress(PressCallback callback);
    
    bool isPressed() const { return pressed; }
    
protected:
    // Implement ComponentTemplate virtual methods
    bool publish(SuperMessage& msg) override;

private:
    const uint8_t pin;
    bool pressed = false;
    bool lastPressed = false;
    
    // Array of callback function pointers
    PressCallback callbacks[MAX_CALLBACKS] = {nullptr};
    size_t callbackCount = 0;

    // SuperMessage fields
    bool& sm_has;
    ButtonMessage& sm_field;
};

#endif // __BUTTON__
