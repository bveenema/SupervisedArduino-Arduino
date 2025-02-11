#ifndef __COMPONENT_TEMPLATE__
#define __COMPONENT_TEMPLATE__

#include "supervisor.h"

class supervisor;

class ComponentTemplate {
public:

    virtual ~ComponentTemplate() = default;

protected:
    friend class supervisor;
    
    // Called by the supervisor after the component notifies it of a change
    // Update the message with this component's data and return true if the message was modified
    virtual bool publish(SuperMessage& msg) = 0;

    // Called by the supervisor before the message is decoded
    // Can be used to set up decode callbacks
    virtual void preReceive(SuperMessage& msg) {}

    // Called by the supervisor when a message is received after it has been decoded
    // Interprets the message and updates the component's state
    virtual void receive(SuperMessage& msg) {}

    // Called by the supervisor to get and set the component id
    size_t getComponentId() const;
    void setComponentId(size_t id);

    // Linked list and component management
    ComponentTemplate* getNext() const;
    void setNext(ComponentTemplate* next);

private:
    size_t componentId = 0;
    ComponentTemplate* nextComponent = nullptr;  // For linked list implementation
}; 



#endif // __COMPONENT_TEMPLATE__
