#include "component_template.h"
#include "supervisor.h"


void ComponentTemplate::markAsUpdated() {
    if (super) {
        super->notifyUpdate(this);
    }
} 

void ComponentTemplate::setComponentId(size_t id) { componentId = id; }

