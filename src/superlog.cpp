#include "superlog.h"
#include "supervisor.h"

char superLog::messageBuffer[256];  // Define static member

superLog::superLog() {
    allowedLevel = superLog::Level::INFO;
    Supervisor.addComponent(this);
}

void superLog::setLevel(superLog::Level level) {
    allowedLevel = level;
}

bool superLog::log(const Level msgLevel, const char *format, ...) {
    // Check if the message level is allowed to be logged
    if (allowedLevel > msgLevel) {
        return false;
    }

    // Format the message
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    // Store in pending message
    pendingLog = SuperLogMessage_init_zero;
    pendingLog.level = (SuperLogLevel)msgLevel;
    pendingLog.message.funcs.encode = &encodeString;
    pendingLog.message.arg = buffer;
    
    // Mark the message as updated
    Supervisor.readyToPublish(this);

    return true;
}

bool superLog::publish(SuperMessage& msg) {
        msg.has_log = true;
        msg.log = pendingLog;
        pendingLog = SuperLogMessage_init_zero;
        return true;
    }

void superLog::preReceive(SuperMessage& msg) {
    msg.log.message.funcs.decode = &decodeString;
    msg.log.message.arg = messageBuffer;
}

void superLog::receive(SuperMessage& msg) {
    if (msg.has_log) {
        // set the new log level
        allowedLevel = (Level)msg.log.level;
        // echo the message
        log((Level)msg.log.level, "Received log: %s", messageBuffer);
    }
}