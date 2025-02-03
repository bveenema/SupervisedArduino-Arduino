#ifndef __LOGGER__
#define __LOGGER__

#include <cstdio>
#include <cstdarg>
#include "pb_encode.h"
#include "logger.pb.h"
#include "pb_main.h"

class ProtoLog {
public:
    friend class supervisor;
    // Log levels
    // These log levels are for use in the Arduino program and map to the log levels in the proto file
    enum Level {
        DEBUG = LogMessageLevel_debug,
        INFO = LogMessageLevel_info,
        WARN = LogMessageLevel_warn,
        ERROR = LogMessageLevel_error,
        FATAL = LogMessageLevel_fatal
    };

    // Constructor
    /// \param main The main message to log to
    /// \param logLevel The default log level, can be updated by setLogLevel
    ProtoLog(supervisor &Supervisor, Level logLevel) : 
        Supervisor(Supervisor),
        allowedLevel(logLevel) {}

    // log
    /// the main logging function. Only logs if the message level is allowed. Uses printf style formatting.
    /// \param msgLevel The level of the message
    /// \param format The format string for the message
    /// \param ... The arguments for the format string
    bool Log(const Level msgLevel, const char *format, ...) {
        // Check if the message level is allowed to be logged
        if (allowedLevel > msgLevel) {
            return false;
        }

        // Format the message
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);

        logger.level = (LogMessageLevel)msgLevel;
        logger.message.funcs.encode = &encode_string;
        logger.message.arg = buffer;

        Supervisor.msg.has_log = true;
        Supervisor.msg.log = logger;

        return true;
    }

    // Semi-private varialbles. These must be public so that the encode_string function can access them but they should NOT be accessed directly
    LogMessage logger = LogMessage_init_zero;
    char buffer[256];
    static bool encode_string(pb_ostream_t *stream, const pb_field_t *field, void * const *arg) {
        const char *str = (const char*)(*arg);
        if (!pb_encode_tag_for_field(stream, field)) {
            return false;
        }
        return pb_encode_string(stream, (const pb_byte_t*)str, strlen(str));
    }

private:
    supervisor &Supervisor;
    // MainMessage &main;
    Level allowedLevel;
};

// Declare singleton instance of the logger
extern ProtoLog Logger;

#endif // __LOGGER__
