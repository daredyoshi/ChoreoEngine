#pragma once

#include <array>
#include "Time.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtx/transform.hpp"

namespace ChoreoApp{

class FloatKey{
public:
    FloatKey() = default;
    FloatKey(float val)
        : m_val{val}, m_time{0} {}
    FloatKey(float val, Time time)
        : m_val{val}, m_time{time} {}

    void setVal(const float val) { m_val= val; }    
    float& getVal() { return m_val; }
    float eval() const { return m_val; }


    operator float (){ return m_val; }
    operator uint32_t() { return m_time; }

protected:
    float m_val;
    Time m_time;
};

template<unsigned int T>
class MultiKey{
public:
    MultiKey() = default;
    MultiKey(std::array<MultiKey, T> vals)
        : m_vals{vals}, m_time{0} {}
    MultiKey(std::array<MultiKey, T> vals, Time time)
        : m_vals{vals}, m_time{time} {}

    void setVal(const float val, unsigned int idx) { m_vals[idx] = val; }    
    std::array<float, T>& getVal() { return m_vals; }
    std::array<float, T> eval() const { return m_vals; }

    void setVals(std::array<MultiKey, T> vals) {m_vals = vals;}

    operator std::array<MultiKey, T>() { return m_vals; }
    operator uint32_t() { return m_time; }

protected:
    std::array<float, T> m_vals;
    Time m_time;
};

class XformKey : public MultiKey<16>{
public:
    XformKey(glm::vec3 p, glm::vec3 r, glm::vec3 s){
        compose(p, r, s);
    };

    void compose(glm::vec3 p, glm::vec3 r, glm::vec3 s){
        glm::mat4 translation = glm::translate(p);
        glm::mat4 rotation = glm::eulerAngleXYZ( r.x, r.y, r.z );
        glm::mat4 scale = glm::scale(s);
        glm::mat4 xform = translation * rotation * scale;
        m_vals = std::array<float, 16>{*glm::value_ptr(xform)};
    };

    glm::vec3 getPosition() const {
        return { m_vals[8], m_vals[9], m_vals[10] }; 
    };

    glm::vec3 getScale() const {
        return {
               glm::length( glm::vec3{m_vals[0], m_vals[1], m_vals[2]} ),
               glm::length( glm::vec3{m_vals[4], m_vals[5], m_vals[6]} ),
               glm::length( glm::vec3{m_vals[8], m_vals[9], m_vals[10]} )
        };
    }
    glm::vec3 getEulerRotation() const {
        float x, y, z;
        glm::extractEulerAngleXYZ(eval(), x, y, z);
        return {x, y, z};
    }
    void setEulerRotation(glm::vec3 eulerAngles){
        glm::vec3 p { getPosition() };
        glm::vec3 s { getScale() };
        compose(p, eulerAngles, s);
    };

    glm::mat4 eval() const { 
        return {
            m_vals[0], 
            m_vals[1], 
            m_vals[2], 
            m_vals[3], 

            m_vals[4], 
            m_vals[5], 
            m_vals[6], 
            m_vals[7], 

            m_vals[8], 
            m_vals[9], 
            m_vals[10], 
            m_vals[11],

            m_vals[12],
            m_vals[13],
            m_vals[14],
            m_vals[15]
        }; 
    }

    void setVals(glm::mat4 val) { 
        const float *pSource = (const float*)glm::value_ptr(val);
        for(unsigned int i{0}; i<16; ++i){
            m_vals[i] = pSource[i]; 
        }
    }    

    operator glm::mat4() { return eval();} 

};


}
