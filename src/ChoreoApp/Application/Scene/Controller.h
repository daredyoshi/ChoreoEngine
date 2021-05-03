#pragma once
#include "capch.h"
#include "Key.h"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include <memory>

namespace ChoreoApp{

template<typename T_returnType>
class Controller{

public:
    Controller() = default;

    T_returnType virtual eval(const Scope<Time>& t) const = 0;
    virtual void setVal(const Scope<Time>& t, T_returnType val) = 0;
    virtual Ref<Key<T_returnType>>& getKey(unsigned int idx) =0;
    virtual Ref<Key<T_returnType>>& getKey(const Scope<Time>& t) =0;
};

template<typename T_returnType>
class AnimatedController : public Controller<T_returnType>{
public:
protected:
    std::vector<Ref<Key<T_returnType>>> m_keys;
};


template<typename T_returnType>
class StepController : public AnimatedController<T_returnType>{

public:
    virtual Ref<Key<T_returnType>>& getKey(unsigned int idx) override { 
        return this->m_keys[idx]; 
    }
    virtual Ref<Key<T_returnType>>& getKey(const Scope<Time>& t) override { 
        unsigned int idx = getPreviousKey(t);
        // will this compile??
        return this->m_keys[idx]; 
    }

    T_returnType virtual eval(const Scope<Time>& t) const override {
        unsigned int idx = getPreviousKey(t);
        return this->m_keys[idx]->eval(); 
    }       

    virtual void setVal(const Scope<Time>& t, T_returnType val) override{
        unsigned int idx = getPreviousKey(t); 
        this->m_keys[idx]->setVal(val);
    }

private:
    unsigned int getPreviousKey(const Scope<Time>& t) const{
        // check to see if this is single value
        if ( this->m_keys.size() == 1){
            return 0;
        }

        for(unsigned int i = 0; i < this->m_keys.size(); ++i){
            Ref<Key<float>> k = this->m_keys[i]; 
            // convert the key to ticks and the time to tcks
            if ((uint32_t)*k >= *t){
                return i-1;
            }
        }
        CE_CORE_ASSERT(false, "Controller did not find keys")
        return 0;
    }
};

template<typename T_returnType>
class StaticController : public Controller<T_returnType>{ 
public:
    StaticController() = default;
    StaticController(T_returnType val){
        m_key = CreateRef<Key<T_returnType>>(val);
    }
    virtual Ref<Key<T_returnType>>& getKey(const Scope<Time>& t) override { 
        (void)t;
        return m_key; 
    }
    virtual Ref<Key<T_returnType>>& getKey(const unsigned int idx) override { 
        (void)idx;
        return m_key; 
    }
    T_returnType virtual eval(const Scope<Time>& t) const override {
        (void)t;
        return this->m_key->eval(); 
    }       

    virtual void setVal(const Scope<Time>& t, T_returnType val) override{
        (void)t;
        this->m_key->setVal(val);
    }

protected:
    Ref<Key<T_returnType>> m_key;
};

// class FloatController : public Controller<float>{};
using FloatController = Controller<float>;
using StaticFloatController = StaticController<float>;
using FloatStepController = StepController<float>;
// class StaticFloatController : public StaticController<float>{};
// class FloatStepController : public StepController<float>{};


class XformController {
public:    
    virtual glm::mat4 eval(const Scope<Time>& t) const =0;
    virtual void setFromMat4(const glm::mat4& xform, Scope<Time>& t) = 0;
};

class EulerXformController : public XformController{

    virtual glm::mat4 eval(const Scope<Time>& t) const override;
    virtual void setFromMat4(const glm::mat4& xform, Scope<Time>& t) override;

    Ref<FloatController>& getXPosController() { return m_xPosController; }
    Ref<FloatController>& getYPosController() { return m_yPosController; }
    Ref<FloatController>& getZPosController() { return m_zPosController; }

    Ref<FloatController>& getXAngleController() { return m_xAngleController; }
    Ref<FloatController>& getYAngleController() { return m_yAngleController; }
    Ref<FloatController>& getZAngleController() { return m_zAngleController; }

    Ref<FloatController>& getXScaleController() { return m_xScaleController; }
    Ref<FloatController>& getYScaleController() { return m_yScaleController; }
    Ref<FloatController>& getZScaleController() { return m_zScaleController; }

private:
    Ref<FloatController> m_xPosController{std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>(0.0f))};
    Ref<FloatController> m_yPosController{std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>(0.0f))};
    Ref<FloatController> m_zPosController{std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>(0.0f))};

    Ref<FloatController> m_xAngleController{std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>(0.0f))};
    Ref<FloatController> m_yAngleController{std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>(0.0f))};
    Ref<FloatController> m_zAngleController{std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>(0.0f))};
                                                                                                                             
    Ref<FloatController> m_xScaleController{std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>(1.0f))};
    Ref<FloatController> m_yScaleController{std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>(1.0f))};
    Ref<FloatController> m_zScaleController{std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>(1.0f))};

    glm::mat4 compose() const;
};



// could try to template this, but since ther is a limited amount of controller types
// I'd rather be explicit here
// class XformStepController {
// public:
//     XformStepController(){
//         m_keys.push_back(Mat4XformKey{});
//     } 
//     XformStepController(Mat4XformKey& k){
//         m_keys.push_back(k);
//     }
//     // XformStepController(glm::vec3 p, glm::vec3 r, glm::vec3 s){
//     //     m_keys.push_back(XformKey{p, r, s});
//     // }
//
//     glm::mat4 virtual eval(const Scope<Time>& t) const {
//         unsigned int idx = getPreviousKey(t);
//         return m_keys[idx].eval(); 
//     }
//     Mat4XformKey& ugetKey(unsigned int idx) { return m_keys[idx]; }
//     Mat4XformKey& getKey(const Scope<Time>& t) {
//         unsigned int idx = getPreviousKey(t);
//         return m_keys[idx]; 
//     }
//
//     void setVal(float val, const Scope<Time>& t, unsigned int xformIdx) { 
//         unsigned int idx = getPreviousKey(t);
//         m_keys[idx].setVal(val, xformIdx); 
//     }
//
//     void setVals(Mat4XformKey val, const Scope<Time>& t) { 
//         unsigned int idx = getPreviousKey(t);
//         m_keys[idx].setVals(val); 
//     }
//
//
// protected:
//     std::vector<Mat4XformKey> m_keys;
// private:
//     unsigned int getPreviousKey(const Scope<Time>& t) const{
//         // check to see if this is single value
//         if ( this->m_keys.size() == 1){
//             return 0;
//         }
//
//         for(unsigned int i = 0; i < this->m_keys.size(); ++i){
//             Mat4XformKey k = this->m_keys[i]; 
//             // convert the key to ticks and the time to tcks
//             if (k >= *t){
//                 return i - 1;
//             }
//         }
//         CE_CORE_ASSERT(false, "Controller did not find keys")
//         return 0;
//     }
// };

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
// this class is for non-animated parameters
// template<unsigned int TLen>
// class MultiStepController{
// public:
//     MultiStepController() {
//         m_keys.push_back(MultiKey<TLen>());
//     };
//     MultiStepController(std::array<float, TLen> val){
//        m_keys.push_back(val);
//     }
//
//     std::array<float, TLen> virtual eval(const Scope<Time>& t) const {
//         unsigned int idx = getPreviousKey(t);
//         return m_keys[idx].eval(); 
//     }
//
//     MultiKey<TLen>& getKey(unsigned int idx) { return m_keys[idx]; }
//     MultiKey<TLen>& getKey(const Scope<Time>& t) {
//         unsigned int idx = getPreviousKey(t);
//         return m_keys[idx]; 
//     }
//
//     void setVal(std::array<float, TLen> val, const Scope<Time>& t) { 
//         unsigned int idx = getPreviousKey(t);
//         m_keys[idx].setVal(val); 
//     }
//
// protected:
//     std::vector<MultiKey<TLen>> m_keys;
//
// private:
//     unsigned int getPreviousKey(const Time& t) const{
//         // check to see if this is single value
//         if ( this->m_keys.size() == 1){
//             return 0;
//         }
//
//         for(unsigned int i = 0; i < this->m_keys.size(); ++i){
//             MultiKey k = this->m_keys[i]; 
//             // convert the key to ticks and the time to tcks
//             if (k >= t){
//                 return i - 1;
//             }
//         }
//         CE_CORE_ASSERT(false, "Controller did not find keys")
//         return 0;
//     }
// };
}
