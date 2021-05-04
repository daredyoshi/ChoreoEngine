#pragma once
#include "capch.h"
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
    void setTime(const Time& t) { m_tick = t.getTick(); }

    operator uint32_t() { return m_tick; }

private:
    uint32_t m_tick{0};
    uint32_t m_ticksPerFrame{30};
    uint32_t m_framesPerSecond{24};
};

class TimeLine{
public:
    const Scope<Time>& getCurrentTime() const {return m_currentTime;}
    const Scope<Time>& getStartTime() const { return m_startTime; }
    const Scope<Time>& getEndTime() const{ return m_endTime; }

    void setCurrentTime(const Time& t) { m_currentTime->setTime(t); }
    void setStartTime(const Time& t) { m_startTime->setTime(t); }
    void setEndTime(const Time& t) { m_endTime->setTime(t); }
private:
    Scope<Time> m_currentTime{CreateScope<Time>(0)};
    Scope<Time> m_startTime{CreateScope<Time>(0)};
    Scope<Time> m_endTime{CreateScope<Time>(7200)};
};

}
