#include "component_template.h"
#include "supervisor.h"


void ComponentTemplate::setComponentId(size_t id) { componentId = id; }

size_t ComponentTemplate::getComponentId() const{ return componentId; }

ComponentTemplate* ComponentTemplate::getNext() const {
    return nextComponent;
}

void ComponentTemplate::setNext(ComponentTemplate* next) {
    nextComponent = next;
}