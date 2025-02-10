#ifndef __COMPONENT_INTERFACE__
#define __COMPONENT_INTERFACE__

#include "supervisor.h"

class supervisor;

class ComponentTemplate {
public:

    virtual ~ComponentTemplate() = default;
    
    // Updates the SuperMessage with this component's data
    // Returns true if the message was modified
    virtual bool updateMessage(SuperMessage& msg) = 0;

    // Handles a message from the supervisor (optional)
    virtual void handleMessage(SuperMessage& msg) {}

    // Simple method to tell supervisor we have new data
    void markAsUpdated();

    size_t getComponentId() const { return componentId; }
    void setComponentId(size_t id);

protected:
    friend class supervisor;
    void setSupervisor(supervisor* sup) {super = sup; }


private:
    supervisor* super = nullptr;
    size_t componentId = 0;
}; 



#endif // __COMPONENT_INTERFACE__
