#pragma once
#include "capch.h"
#include "Key.h"
#include "glm/glm.hpp"

namespace ChoreoApp{

class Scene;

// pure virtual base class
template<typename T>
class Controller{
public:
    Controller() = default;

    T virtual eval() const = 0;
private:
};

// float is purely semantic here
// this returns double to minimize floating point errors
class FloatController : public Controller<double>{
public:
    FloatController() = default;
    FloatController(double val) 
        : m_key{val} {}

    double virtual eval() const override { return m_key.eval(); };
    FloatKey& getKey() { return m_key; }
    void setVal(double val) { m_key.setVal( val); };

    operator double(){ return eval(); }
private:
    FloatKey m_key; 
};

class LinearFloatController : public FloatController{
public:
    LinearFloatController(Scope<Scene>& scene)
        : m_scene{scene}  {}

    double virtual eval() const override; 
    FloatKey& getKey(uint32_t i) { return m_keys[i]; }
    void setKeyVal(uint32_t i, double val) { m_keys[i].setVal(val); }

private:
    Scope<Scene>& m_scene;
    std::vector<FloatKey> m_keys;
};

class StaticTransformController : public Controller<glm::mat4>{
    
};

}
