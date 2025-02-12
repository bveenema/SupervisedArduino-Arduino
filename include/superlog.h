#ifndef __SUPERLOG__
#define __SUPERLOG__

#include <cstdio>
#include <cstdarg>
#include "pb_encode.h"
#include "superlog.pb.h"
#include "component_template.h"


/**
 * @brief Macro that provides access to the singleton instance of superLog.
 * 
 * This macro wraps the getInstance() call to provide a simpler interface.
 * Instead of calling superLog::getInstance(), users can simply use SuperLog.
 */
#define SuperLog superLog::getInstance()

class superLog : public ComponentTemplate {
public:
    /**
     * Log levels that map to corresponding levels defined in the proto file.
     * Used for filtering and categorizing log messages in the Arduino program.
     */
    enum Level {
        DEBUG = SuperLogLevel_debug,
        INFO = SuperLogLevel_info,
        WARN = SuperLogLevel_warn,
        ERROR = SuperLogLevel_error,
        FATAL = SuperLogLevel_fatal
    };

    /**
     * @brief Get the singleton instance of superLog.
     * 
     * @return superLog& The singleton instance of superLog.
     */
    static superLog& getInstance() {
        static superLog instance;
        return instance;
    }


    /**
     * @brief Set the log level.
     * 
     * @param level The log level to allow, defaults to info
     */
    void setLevel(Level level);


    /**
     * @brief The main logging function. Only logs if the message level is allowed.
     * 
     * Uses printf style formatting.
     * 
     * @param msgLevel The level of the message
     * @param format The format string for the message
     * @param ... The arguments for the format string
     * @return true if the message was logged, false if it was filtered
     */
    bool log(const Level msgLevel, const char *format, ...);


    bool publish(SuperMessage& msg) override;
    void receive(SuperMessage& msg) override;

private:
    superLog();
    superLog(const superLog&) = delete;
    superLog& operator=(const superLog&) = delete;

    /**
     * @brief Resets the pending log message to its initial state
     */
    void resetPendingLog();

    Level allowedLevel;
    SuperLogMessage pendingLog = SuperLogMessage_init_zero;
    char buffer[64];
};


#endif // __SUPERLOG__
