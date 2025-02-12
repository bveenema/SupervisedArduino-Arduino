#ifndef __SUPERVISOR__
#define __SUPERVISOR__

#include <pb_arduino.h>
#include "supervisor.pb.h"
#include "component_template.h"

class ComponentTemplate;

class supervisor {
public:
    static supervisor& getInstance() {
        static supervisor instance;
        return instance;
    }

    void update();
    void addComponent(ComponentTemplate* component);
    void readyToPublish(ComponentTemplate* component);

    SuperMessage outMsg = SuperMessage_init_zero;
    SuperMessage inMsg = SuperMessage_init_zero;

    // Maximum number of components we'll support
    static constexpr size_t MAX_COMPONENTS = 64;

private:
    supervisor();
    supervisor(const supervisor&) = delete;
    supervisor& operator=(const supervisor&) = delete;

    void resetSuperMessage(SuperMessage& msg);

    pb_ostream_s pb_out;
    uint64_t pendingUpdates = 0;  // Each bit represents one component
    ComponentTemplate* head = nullptr;  // Head of the linked list
    size_t componentCount = 0;
};

#define Supervisor supervisor::getInstance()

#endif // __SUPERVISOR__
