#pragma once
#include "capch.h"
#include <bits/stdint-uintn.h>

namespace ChoreoApp{

class Time{
public:
    Time() = default; 
    Time( uint32_t frame) {
        setFrame(frame);
    }
    float getSecond() const { return (float)m_tick / (float)m_ticksPerFrame / (float)m_framesPerSecond; };
    float getFrame() const { return (float)(m_tick / (float)m_ticksPerFrame); };
    uint32_t getTick() const { return m_tick; };

    void setSecond(float const sec){ m_tick = sec * (float)m_framesPerSecond * (float)m_ticksPerFrame; }
    void setFrame(float const frame){ m_tick = frame* (float)m_ticksPerFrame; }
    void setTick(int const tick){ m_tick = tick; }
    void setTime(const Time& t) { m_tick = t.getTick(); }

    static uint32_t getFramesPerSecond() { return m_framesPerSecond; }
    static uint32_t getTicksPerFrame() { return m_ticksPerFrame; }


private:
    uint32_t m_tick{0};
    static uint32_t m_ticksPerFrame;
    static uint32_t m_framesPerSecond;
};


class TimeLine{
public:
    const Time& getCurrentTime() const {return m_currentTime;}
    const Time& getStartTime() const { return m_startTime; }
    const Time& getEndTime() const{ return m_endTime; }

    void setCurrentTime(const Time& t) { m_currentTime.setTime(t); }
    void setStartTime(const Time& t) { m_startTime.setTime(t); }
    void setEndTime(const Time& t) { m_endTime.setTime(t); }
private:
    Time m_currentTime{0};
    Time m_startTime{0};
    Time m_endTime{240};
};

}
