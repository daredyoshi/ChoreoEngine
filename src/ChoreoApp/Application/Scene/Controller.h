#pragma once
#include "capch.h"
#include "Key.h"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"

namespace ChoreoApp{


// this class is for non-animated parameters
template<unsigned int TLen>
class StepController{
public:
    StepController() = default;
    StepController(std::array<float, TLen> val){
       m_keys.push_back(val);
    }

    std::array<float, TLen> virtual eval(const Scope<Time>& t) const {
        unsigned int idx = getPreviousKey(t);
        return m_keys[idx].eval(); 
    }

    Key<TLen>& getKey(unsigned int idx) { return m_keys[idx]; }
    Key<TLen>& getKey(const Scope<Time>& t) {
        unsigned int idx = getPreviousKey(t);
        return m_keys[idx]; 
    }

    void setVal(std::array<float, TLen> val, unsigned int idx ) { m_keys[idx].setVal(val); }
    void setVal(std::array<float, TLen> val, const Scope<Time>& t) { 
        unsigned int idx = getPreviousKey(t);
        m_keys[idx].setVal(val); 
    }

protected:
    std::vector<Key<TLen>> m_keys;

private:
    unsigned int getPreviousKey(const Time& t) const{
        // check to see if this is single value
        if ( this->m_keys.size() == 1){
            return 0;
        }

        for(unsigned int i = 0; i < this->m_keys.size(); ++i){
            Key k = this->m_keys[i]; 
            // convert the key to ticks and the time to tcks
            if (k > t){
                return i;
            }
        }
        CE_CORE_ASSERT(false, "Controller did not find keys")
        return 0;
    }
};


// could try to template this, but since ther is a limited amount of controller types
// I'd rather be explicit here
class XformStepController {
    XformStepController() = default;
    XformStepController(glm::vec3 p, glm::vec3 r, glm::vec3 s){
        m_keys.push_back(XformKey{p, r, s});
    }

    glm::mat4 virtual eval(const Scope<Time>& t) const {
        unsigned int idx = getPreviousKey(t);
        return m_keys[idx].eval(); 
    }
    XformKey& getKey(unsigned int idx) { return m_keys[idx]; }
    XformKey& getKey(const Scope<Time>& t) {
        unsigned int idx = getPreviousKey(t);
        return m_keys[idx]; 
    }

    void setVal(float val, unsigned int keyIdx, unsigned int xformIdx) { m_keys[keyIdx].setVal(val, xformIdx); }
    void setVal(float val, const Scope<Time>& t, unsigned int xformIdx) { 
        unsigned int idx = getPreviousKey(t);
        m_keys[idx].setVal(val, xformIdx); 
    }

    void setVals(XformKey val, unsigned int keyIdx) { m_keys[keyIdx].setVals(val); }
    void setVals(XformKey val, const Scope<Time>& t) { 
        unsigned int idx = getPreviousKey(t);
        m_keys[idx].setVals(val); 
    }


protected:
    std::vector<XformKey> m_keys;
private:
    unsigned int getPreviousKey(const Scope<Time>& t) const{
        // check to see if this is single value
        if ( this->m_keys.size() == 1){
            return 0;
        }

        for(unsigned int i = 0; i < this->m_keys.size(); ++i){
            Key k = this->m_keys[i]; 
            // convert the key to ticks and the time to tcks
            if (k > *t){
                return i;
            }
        }
        CE_CORE_ASSERT(false, "Controller did not find keys")
        return 0;
    }
};

// float is purely semantic here
// this returns float to minimize floating point errors
// using FloatController = Controller<float>;

// using Float3Controller = MultiController<float, 3>;



// class LinearFloatController : public FloatController{
// public:
//     LinearFloatController(Scope<Scene>& scene)
//         : m_scene{scene}  {}
//
//     float virtual eval() const override; 
//     FloatKey& getKey(uint32_t i) { return m_keys[i]; }
//     void setKeyVal(uint32_t i, float val) { m_keys[i].setVal(val); }
//
// };
//
    
// };

// using XformController = Controller<glm::mat4>;
//
// class EulerXformController : public XformController{
// public:
//
//
// private:
//     glm::vec3 m_eulerAngles
//
// }
//
}
