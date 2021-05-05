#pragma once

#include <array>
#include "Time.h"
#include "capch.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtx/transform.hpp"

namespace ChoreoApp{

template<typename T_returnType>
class Key{
public:
    Key() = default;
    Key(Time time)
        : m_time{time} {}
    Key(Time time, T_returnType val)
        : m_time{ time }, m_val{val} {}

    T_returnType eval() const { return m_val; }; 
    void setVal(const T_returnType val) { m_val= val; }    
    T_returnType& getVal() { return m_val; }
    unsigned int getTick() { return m_time.getTick(); }

    operator T_returnType() { return m_val; }

protected:
    Time m_time;
    T_returnType m_val;
};

//
// class Mat4XformKey : public XformKey{
// public:
//     Mat4XformKey(){
//         setVals(glm::mat4{ 1.0 });
//     }
//     Mat4XformKey(glm::vec3 p, glm::vec3 r, glm::vec3 s){
//         compose(p, r, s);
//     };
//     Mat4XformKey( glm::mat4 m ){
//         setVals(m);
//     };
//
//     void compose(glm::vec3 p, glm::vec3 r, glm::vec3 s){
//         glm::mat4 translation = glm::translate(p);
//         glm::mat4 rotation = glm::eulerAngleXYZ( glm::radians(r.x), glm::radians(r.y),glm::radians( r.z ));
//         glm::mat4 xform = translation * rotation;
//
//         setVals( xform );
//         setScale(s);
//     };
//
//     glm::vec3 getPosition() const {
//         return { m_vals[12], m_vals[13], m_vals[14] }; 
//     };
//
//     void setPosition(glm::vec3 p){
//         m_vals[12] = p.x;
//         m_vals[13] = p.y;
//         m_vals[14] = p.z;
//     }
//
//     glm::vec3 getScale() const {
//         return {
//                glm::length( glm::vec3{m_vals[0], m_vals[1], m_vals[2]} ),
//                glm::length( glm::vec3{m_vals[4], m_vals[5], m_vals[6]} ),
//                glm::length( glm::vec3{m_vals[8], m_vals[9], m_vals[10]} )
//         };
//     }
//     void setScale(glm::vec3 s){
//         // no negative scale allowed
//         if (s.x <=0){
//             s.x = 0.000001;
//         }
//         if (s.y<=0){
//             s.y = 0.000001;
//         }
//         if (s.z<=0){
//             s.z = 0.000001;
//         }
//         glm::vec3 x =  glm::normalize( glm::vec3{m_vals[0], m_vals[1], m_vals[2]} ) * s.x;
//         glm::vec3 y =  glm::normalize( glm::vec3{m_vals[4], m_vals[5], m_vals[6]} ) * s.y;
//         glm::vec3 z =  glm::normalize( glm::vec3{m_vals[8], m_vals[9], m_vals[10]} ) * s.z;
//        
//         m_vals[0] = x.x; 
//         m_vals[1] = x.y; 
//         m_vals[2] = x.z; 
//         m_vals[4] = y.x; 
//         m_vals[5] = y.y; 
//         m_vals[6] = y.z; 
//         m_vals[8] = z.x; 
//         m_vals[9] = z.y; 
//         m_vals[10] = z.z; 
//     }
//
//     glm::vec3 getEulerRotation() const {
//         float x, y, z;
//         glm::extractEulerAngleXYZ(eval(), x, y, z);
//         return {glm::degrees(x),glm::degrees( y),glm::degrees( z)};
//     }
//     void setEulerRotation(glm::vec3 r){
//         glm::vec3 p { getPosition() };
//         glm::vec3 s { getScale() };
//         compose(p, r, s);
//     };
//
//     glm::mat4 eval() const { 
//         return {
//             m_vals[0], 
//             m_vals[1], 
//             m_vals[2], 
//             m_vals[3], 
//
//             m_vals[4], 
//             m_vals[5], 
//             m_vals[6], 
//             m_vals[7], 
//
//             m_vals[8], 
//             m_vals[9], 
//             m_vals[10], 
//             m_vals[11],
//
//             m_vals[12],
//             m_vals[13],
//             m_vals[14],
//             m_vals[15]
//         }; 
//     }
//
//     void setVals(glm::mat4 val) { 
//         // CE_CORE_TRACE("Setting Matrix Vals: ");
//         std::array<float,16> dArray= {0.0};
//
//         const float *pSource = (const float*)glm::value_ptr(val);
//         for (int i = 0; i < 16; ++i){
//             // CE_CORE_TRACE("{0}", pSource[i]);
//             dArray[i] = pSource[i];
//         }
//         m_vals = dArray;
//     }    
//
//
// };

// template<unsigned int T>
// class MultiKey{
// public:
//     MultiKey() = default;
//     MultiKey(std::array<MultiKey, T> vals)
//         : m_vals{vals}, m_time{0} {}
//     MultiKey(std::array<MultiKey, T> vals, Time time)
//         : m_vals{vals}, m_time{time} {}
//
//     void setVal(const float val, unsigned int idx) { m_vals[idx] = val; }    
//     std::array<float, T>& getVal() { return m_vals; }
//     std::array<float, T> eval() const { return m_vals; }
//
//     void setVals(std::array<MultiKey, T> vals) {m_vals = vals;}
//
//     operator std::array<MultiKey, T>() { return m_vals; }
//     operator uint32_t() { return m_time; }
//
// protected:
//     std::array<float, T> m_vals{};
//     Time m_time{0};
// };


}
