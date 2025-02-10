#include "superlog.h"
#include "supervisor.h"

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
    pendingLog.message.funcs.encode = &encode_string;
    pendingLog.message.arg = buffer;
    
    // Mark the message as updated
    markAsUpdated();

    return true;
}