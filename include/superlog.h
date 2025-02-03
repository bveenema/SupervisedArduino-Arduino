#ifndef __SUPERLOG__
#define __SUPERLOG__

#include <cstdio>
#include <cstdarg>
#include "pb_encode.h"
#include "superlog.pb.h"

#define SuperLog superLog::getInstance()

class superLog {
public:
    // Log levels
    // These log levels are for use in the Arduino program and map to the log levels in the proto file
    enum Level {
        DEBUG = SuperLogLevel_debug,
        INFO = SuperLogLevel_info,
        WARN = SuperLogLevel_warn,
        ERROR = SuperLogLevel_error,
        FATAL = SuperLogLevel_fatal
    };

    static superLog& getInstance() {
        static superLog instance;
        return instance;
    }

    // setLevel
    /// \param level - The log level to allow, defaults to info
    void setLevel(Level level);

    // log
    /// the main logging function. Only logs if the message level is allowed. Uses printf style formatting.
    /// \param msgLevel The level of the message
    /// \param format The format string for the message
    /// \param ... The arguments for the format string
    bool log(const Level msgLevel, const char *format, ...);


private:
    // These are sent by pointer to Supervisor, which gets around the private status of them.
    // It's a bit of a hack but they are at least protected from general use.
    SuperLogMessage logger = SuperLogMessage_init_zero;
    char buffer[256];
    static bool encode_string(pb_ostream_t *stream, const pb_field_t *field, void * const *arg) {
        const char *str = (const char*)(*arg);
        if (!pb_encode_tag_for_field(stream, field)) {
            return false;
        }
        return pb_encode_string(stream, (const pb_byte_t*)str, strlen(str));
    }

private:
    superLog();
    superLog(const superLog&) = delete;
    superLog& operator=(const superLog&) = delete;

    Level allowedLevel;
};


#endif // __SUPERLOG__
