#pragma once
#include "Time.h"

namespace ChoreoApp{

template<typename T>
class Key{
public:
    Key() = default;
    Key(T val)
        : m_val{val}, m_time{0} {}
    Key(T val, Time time)
        : m_val{val}, m_time{time} {}
    virtual ~Key(){};

    void setVal(const T val) { m_val = val; }    
    T& getVal() { return m_val; }
    T eval() const { return m_val; }

protected:
    T m_val;
    Time m_time;
};

class FloatKey : public Key<double>{
public:
    FloatKey() = default;
    FloatKey(double val)
        : Key{val} {}

    operator double() { return m_val; }
    operator uint32_t() { return m_time; }
};

}
