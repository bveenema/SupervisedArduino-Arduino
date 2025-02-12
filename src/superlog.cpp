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

    // Store in pending message array if there's space
    if (pendingLog.logs_count >= 5) {
        // Buffer is full, force a publish
        Supervisor.readyToPublish(this);
        return false;
    }

    // Add new message to the logs array
    LogMessage* newMsg = &pendingLog.logs[pendingLog.logs_count];
    newMsg->level = (SuperLogLevel)msgLevel;
    strncpy(newMsg->message, buffer, sizeof(newMsg->message) - 1);
    newMsg->message[sizeof(newMsg->message) - 1] = '\0';
    
    pendingLog.logs_count++;
    pendingLog.currentLevel = (SuperLogLevel)msgLevel;
    
    // Mark the message as updated
    Supervisor.readyToPublish(this);

    return true;
}

void superLog::resetPendingLog() {
    // Zero out the entire structure
    memset(&pendingLog, 0, sizeof(SuperLogMessage));
    // Only need to set currentLevel since logs_count is already 0 from memset
    pendingLog.currentLevel = (SuperLogLevel)allowedLevel;
}

bool superLog::publish(SuperMessage& msg) {
    msg.has_log = true;
    msg.log = pendingLog;
    resetPendingLog();
    return true;
}

void superLog::receive(SuperMessage& msg) {
    if (msg.has_log) {
        // set the new log level
        allowedLevel = (Level)msg.log.currentLevel;
        
        // echo the received messages
        for (pb_size_t i = 0; i < msg.log.logs_count; i++) {
            log((Level)msg.log.logs[i].level, "Received log: %s", msg.log.logs[i].message);
        }
    }
}