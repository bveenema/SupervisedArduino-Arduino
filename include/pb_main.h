#ifndef __PB_MAIN__
#define __PB_MAIN__

#include <pb_arduino.h>
#include "main.pb.h"

class supervisor {
public:
    supervisor();
    void Send();

    MainMessage msg;

private:
    pb_ostream_s pb_out;
};

extern supervisor Supervisor;

#endif // __PB_MAIN__
