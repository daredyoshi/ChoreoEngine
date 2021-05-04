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
    // if a key exists at time t, this will set it, otherwise
    // it will only set the cached value which will be cleared on
    // slidertime change
    virtual void setValAtTime(const Scope<Time>& t, T_returnType val) = 0;
    virtual Ref<Key<T_returnType>>& getKeyFromIdx(unsigned int idx) =0;
    // this will get a key at time t if it exists 
    virtual Ref<Key<T_returnType>>& getKeyFromTime(const Scope<Time>& t) =0;

    virtual void dirty() =0;
};

template<typename T_returnType>
class StaticController : public Controller<T_returnType>{ 
public:
    StaticController() = default;
    StaticController(T_returnType val){
        m_key->setVal(val);
        // m_key = CreateRef<Key<T_returnType>>(val);
    }
    virtual Ref<Key<T_returnType>>& getKeyFromTime(const Scope<Time>& t) override { 
        (void)t;
        return m_key; 
    }
    virtual Ref<Key<T_returnType>>& getKeyFromIdx(const unsigned int idx) override { 
        (void)idx;
        return m_key; 
    }
    T_returnType virtual eval(const Scope<Time>& t) const override {
        (void)t;
        return this->m_key->eval(); 
    }       

    virtual void setValAtTime(const Scope<Time>& t, T_returnType val) override{
        (void)t;
        this->m_key->setVal(val);
    }

    virtual void dirty() override {};

protected:
    Ref<Key<T_returnType>> m_key{CreateRef<Key<T_returnType>>()};
};


template<typename T_returnType>
class AnimatedController : public Controller<T_returnType>{
protected:
    std::vector<Ref<Key<T_returnType>>> m_keys;
    T_returnType m_cache;
    bool m_dirty{true};

public:
    virtual Ref<Key<T_returnType>>& getKey(unsigned int idx) override { 
        return this->m_keys[idx]; 
    }
    std::vector<Ref<Key<T_returnType>>>& getKeys(){
        return m_keys;
    }


    
    virtual Ref<Key<T_returnType>>& getKeyFromIdx(unsigned int idx) const override{
        return m_keys[idx];
    };
    virtual Ref<Key<T_returnType>>& getKeyFromTime(const Scope<Time>& t) const override{
        
        // check to see if this is single value
        if ( this->m_keys.size() == 1){
            return m_keys[0];
        }

        for(unsigned int i = 0; i < this->m_keys.size(); ++i){
            Ref<Key<T_returnType>> k = this->m_keys[i]; 

            if (k->getTick() == t->getTick()){
                return k;
            }
            if (k->getTick() > t->getTick()){
                return nullptr;
            }
        }
        CE_CORE_ASSERT(false, "Controller did not find keys")
        return 0;
    }

    unsigned int getPreviousKeyIdx(const Scope<Time>& t) const override {
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

    T_returnType virtual eval(const Scope<Time>& t) override {
        if(m_dirty){
            unsigned int idx = getPreviousKeyIdx(t);

            // default to stepped interpolation
            m_cache = m_keys[idx]->eval();
            m_dirty = false;
        } 
        return m_cache; 
    }       
    virtual void setValAtTime(const Scope<Time>& t, T_returnType val) {
        unsigned int idx = getPreviousKeyIdx(t);
        Ref<Key<T_returnType>>& k = getKeyFromTime(t);
        if(k){
            k->setVal(val);
        }
        m_cache = val;
    }

    virtual void dirty() { m_dirty = true; }
};

// class FloatController : public Controller<float>{};
using FloatController = Controller<float>;
using StaticFloatController = StaticController<float>;
using FloatAnimatedController = AnimatedController<float>;
// class StaticFloatController : public StaticController<float>{};
// class FloatStepController : public StepController<float>{};


class XformController {
public:    
    enum class XformType{
        Euler = 0,
        Quaternion = 1,
        Script = 2
    };

    XformController(XformType t)
        : m_type{t} {}

    virtual XformType getType() const  { return m_type; }

    virtual glm::mat4 eval(const Scope<Time>& t) =0;
    virtual void setFromMat4(const glm::mat4& xform, Scope<Time>& t) = 0;

    virtual glm::vec3 evalPosition(const Scope<Time>& t) const;
    virtual glm::vec3 evalEulerAngles(const Scope<Time>& t) const = 0;
    virtual glm::vec3 evalScale(const Scope<Time>& t) const;
    // this will set the key at the time, or create one
    virtual void setPositionAtTime(const Scope<Time>& t, const glm::vec3& p);
    virtual void setEulerAnglesAtTime(const Scope<Time>& t, const glm::vec3& r)  = 0;
    virtual void setScaleAtTime(const Scope<Time>& t, const glm::vec3& s);

    Ref<FloatController>& getXPosController() { return m_xPosController; }
    Ref<FloatController>& getYPosController() { return m_yPosController; }
    Ref<FloatController>& getZPosController() { return m_zPosController; }

    Ref<FloatController>& getXScaleController() { return m_xScaleController; }
    Ref<FloatController>& getYScaleController() { return m_yScaleController; }
    Ref<FloatController>& getZScaleController() { return m_zScaleController; }

    virtual void dirty();

protected:
    XformType m_type;
    glm::mat4 m_cache;
    bool m_dirty;

    Ref<FloatController> m_xPosController{std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>(0.0f))};
    Ref<FloatController> m_yPosController{std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>(0.0f))};
    Ref<FloatController> m_zPosController{std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>(0.0f))};

    Ref<FloatController> m_xScaleController{std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>(1.0f))};
    Ref<FloatController> m_yScaleController{std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>(1.0f))};
    Ref<FloatController> m_zScaleController{std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>(1.0f))};

};

class EulerXformController : public XformController{
public:
    EulerXformController()
        : XformController(XformController::XformType::Euler) {}

    virtual glm::mat4 eval(const Scope<Time>& t) override;
    virtual void setFromMat4(const glm::mat4& xform, Scope<Time>& t) override;

    virtual glm::vec3 evalEulerAngles(const Scope<Time>& t) const override;

    // this will set the key at the time, or create one
    virtual void setEulerAnglesAtTime(const Scope<Time>& t, const glm::vec3& r) override;


    Ref<FloatController>& getXAngleController() { return m_xAngleController; }
    Ref<FloatController>& getYAngleController() { return m_yAngleController; }
    Ref<FloatController>& getZAngleController() { return m_zAngleController; }

    virtual void dirty() override;

private:
    Ref<FloatController> m_xAngleController{std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>(0.0f))};
    Ref<FloatController> m_yAngleController{std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>(0.0f))};
    Ref<FloatController> m_zAngleController{std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>(0.0f))};
};

// class QuaternionXformController : public XformController{
//     
// };
}
