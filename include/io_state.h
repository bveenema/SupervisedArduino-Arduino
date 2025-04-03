#ifndef __IO_STATE__
#define __IO_STATE__

#include <cstdint>
#include "pb_encode.h"
#include "io_state.pb.h"
#include "component_template.h"

/**
 * @brief Macro that provides access to the singleton instance of iostate.
 * 
 * This macro wraps the getInstance() call to provide a simpler interface.
 * Instead of calling iostate::getInstance(), users can simply use IO.
 */
#define IOState iostate::getInstance()

class iostate : public ComponentTemplate {
public:
    /**
     * @brief Get the singleton instance of iostate.
     * 
     * @return iostate& The singleton instance of iostate.
     */
    static iostate& getInstance() {
        static iostate instance;
        return instance;
    }

    // Digital IO getters/setters
    bool getIO0() const { return state.p_IO0; }
    bool getIO1() const { return state.p_IO1; }
    bool getIO2() const { return state.p_IO2; }
    bool getIO3() const { return state.p_IO3; }
    bool getIO4() const { return state.p_IO4; }
    bool getIO5() const { return state.p_IO5; }
    bool getDI6() const { return state.p_DI6; }
    bool getDI7() const { return state.p_DI7; }
    bool getDI8() const { return state.p_DI8; }

    void setIO0(bool value);
    void setIO1(bool value);
    void setIO2(bool value);
    void setIO3(bool value);
    void setIO4(bool value);
    void setIO5(bool value);

    // Analog IO getters/setters
    uint32_t getA9() const { return state.p_A9; }
    uint32_t getA10() const { return state.p_A10; }
    uint32_t getA11() const { return state.p_A11; }
    uint32_t getA12() const { return state.p_A12; }

    void setA9(uint32_t value);
    void setA10(uint32_t value);
    void setA11(uint32_t value);
    void setA12(uint32_t value);

    bool publish(SuperMessage& msg) override;
    void receive(SuperMessage& msg) override;

private:
    iostate();
    iostate(const iostate&) = delete;
    iostate& operator=(const iostate&) = delete;

    iostateMessage state = iostateMessage_init_zero;
};

#endif // __IO_STATE__
