#ifndef __SUPERVISOR__
#define __SUPERVISOR__

#include <pb_arduino.h>
#include "supervisor.pb.h"
#include "component_template.h"

// Undefine Arduino's min/max macros
#undef min
#undef max

#include <vector>
#include <bitset>

class ComponentTemplate;

class supervisor {
public:
    static supervisor& getInstance() {
        static supervisor instance;
        return instance;
    }

    void send();
    void update();
    void addComponent(ComponentTemplate* provider);
    void notifyUpdate(ComponentTemplate* provider);


    SuperMessage outMsg = SuperMessage_init_zero;
    SuperMessage inMsg = SuperMessage_init_zero;

    // Maximum number of providers we'll support
    static constexpr size_t MAX_PROVIDERS = 64;

private:
    supervisor();
    supervisor(const supervisor&) = delete;
    supervisor& operator=(const supervisor&) = delete;

    pb_ostream_s pb_out;
    std::bitset<MAX_PROVIDERS> pendingUpdates;  // Each bit represents one provider
    std::vector<ComponentTemplate*> providers;    // Index in this array corresponds to bit position
};

#define Supervisor supervisor::getInstance()

#endif // __SUPERVISOR__