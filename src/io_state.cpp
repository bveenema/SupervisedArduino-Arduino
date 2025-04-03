#include "io_state.h"
#include "supervisor.h"

iostate::iostate() {
    state = iostateMessage_init_zero;
    Supervisor.addComponent(this);
}

// Digital IO setters
void iostate::setIO0(bool value) {
    if (state.p_IO0 != value) {
        state.p_IO0 = value;
        Supervisor.readyToPublish(this);
    }
}

void iostate::setIO1(bool value) {
    if (state.p_IO1 != value) {
        state.p_IO1 = value;
        Supervisor.readyToPublish(this);
    }
}

void iostate::setIO2(bool value) {
    if (state.p_IO2 != value) {
        state.p_IO2 = value;
        Supervisor.readyToPublish(this);
    }
}

void iostate::setIO3(bool value) {
    if (state.p_IO3 != value) {
        state.p_IO3 = value;
        Supervisor.readyToPublish(this);
    }
}

void iostate::setIO4(bool value) {
    if (state.p_IO4 != value) {
        state.p_IO4 = value;
        Supervisor.readyToPublish(this);
    }
}

void iostate::setIO5(bool value) {
    if (state.p_IO5 != value) {
        state.p_IO5 = value;
        Supervisor.readyToPublish(this);
    }
}

// Analog IO setters
void iostate::setA9(uint32_t value) {
    if (state.p_A9 != value) {
        state.p_A9 = value;
        Supervisor.readyToPublish(this);
    }
}

void iostate::setA10(uint32_t value) {
    if (state.p_A10 != value) {
        state.p_A10 = value;
        Supervisor.readyToPublish(this);
    }
}

void iostate::setA11(uint32_t value) {
    if (state.p_A11 != value) {
        state.p_A11 = value;
        Supervisor.readyToPublish(this);
    }
}

void iostate::setA12(uint32_t value) {
    if (state.p_A12 != value) {
        state.p_A12 = value;
        Supervisor.readyToPublish(this);
    }
}

bool iostate::publish(SuperMessage& msg) {
    msg.has_io_state = true;
    msg.io_state = state;
    return true;
}

void iostate::receive(SuperMessage& msg) {
    if (msg.has_io_state) {
        state = msg.io_state;
    }
} 