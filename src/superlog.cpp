#include "superlog.h"
#include "supervisor.h"


superLog::superLog() {
    allowedLevel = superLog::Level::INFO;
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

    logger.level = (SuperLogLevel)msgLevel;
    logger.message.funcs.encode = &encode_string;
    logger.message.arg = buffer;

    Supervisor.msg.has_log = true;
    Supervisor.msg.log = logger;

    return true;
}