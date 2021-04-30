#pragma once

#include <bits/stdint-uintn.h>
namespace ChoreoApp{

class Time{
public:
    Time() = default; 
    Time( uint32_t tick) : m_tick{tick} {}
    float getSecond() const { return (float)m_tick / (float)m_ticksPerFrame / (float)m_framesPerSecond; };
    float getFrame() const { return (float)(m_tick * (float)m_ticksPerFrame); };
    uint32_t getTick() const { return m_tick; };

    void setSecond(float const sec){ m_tick = sec * (float)m_framesPerSecond * (float)m_ticksPerFrame; }
    void setFrame(float const frame){ m_tick = frame* (float)m_ticksPerFrame; }
    void setTick(int const tick){ m_tick = tick; }

    operator uint32_t() { return m_tick; }

private:
    uint32_t m_tick{0};
    uint32_t m_ticksPerFrame{30};
    uint32_t m_framesPerSecond{24};
};

}
