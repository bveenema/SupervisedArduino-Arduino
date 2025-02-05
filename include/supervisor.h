#ifndef __PB_MAIN__
#define __PB_MAIN__

#include <pb_arduino.h>
#include "supervisor.pb.h"

#define Supervisor supervisor::getInstance()
class supervisor {
public:
    static supervisor& getInstance() {
        static supervisor instance;
        return instance;
    }

    void send();
    void update();

    SuperMessage msg = SuperMessage_init_zero;

private:
    supervisor();
    supervisor(const supervisor&) = delete;
    supervisor& operator=(const supervisor&) = delete;

    pb_ostream_s pb_out;
};

#endif // __PB_MAIN__