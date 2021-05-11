#pragma once
#include "Application/Scene/Scene.h"
#include "capch.h"
#include "Key.h"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include <memory>

namespace ChoreoApp{


class Scene;

class FloatController{

public:
    enum class ControllerType{
        Static = 0,
        Animated = 1,
        Script = 2
    };
    FloatController(ControllerType t, std::weak_ptr<Scene> scene, const std::string& label="No Name");
    virtual ControllerType getType() const  { return m_type; }

    float virtual eval(const Time& t) = 0;
    // if a key exists at time t, this will set it, otherwise
    // it will only set the cached value which will be cleared on
    // slidertime change
    virtual void setValAtTime(const Time& t, float val) = 0;
    virtual Ref<FloatKey> getKeyFromIdx(unsigned int idx) =0;
    // this will get a key at time t if it exists 
    virtual Ref<FloatKey> getKeyFromTime(const Time& t) =0;

    virtual std::vector<Ref<FloatKey>> getKeys() =0;

    virtual void addKey(Ref<FloatKey> key) =0;

    const std::string& getLabel() const { return m_label; }
    void setLabel(const std::string& label) { m_label = label; }

    virtual uint32_t getID() const { return m_id; }
    virtual void dirty() =0;


private:
    ControllerType m_type;
    std::string m_label{};
    uint32_t m_id;
};

class StaticFloatController : public FloatController{ 
public:
    StaticFloatController(std::weak_ptr<Scene> scene, const std::string& label="No Name");
    StaticFloatController(std::weak_ptr<Scene> scene, float val, const std::string& label="No Name" );

    float virtual eval(const Time& t) override {(void)t; return this->m_key->eval(); }       

    virtual Ref<Key<float>> getKeyFromTime(const Time& t) override { (void)t;return m_key; }
    virtual Ref<Key<float>> getKeyFromIdx(const unsigned int idx) override { (void)idx; return m_key; }

    virtual std::vector<Ref<Key<float>>> getKeys() override { return {{ m_key }}; };

    virtual void addKey(Ref<Key<float>> key)override { m_key = key; };

    virtual void setValAtTime(const Time& t, float val) override{(void)t; this->m_key->setVal(val);}

    virtual void dirty() override {};

protected:
    Ref<FloatKey> m_key{CreateRef<FloatKey>()};
};


class AnimatedFloatController : public FloatController{
protected:
    std::vector<Ref<FloatKey>> m_keys;
    float m_cache;
    bool m_dirty{true};

public:
    AnimatedFloatController(std::weak_ptr<Scene> scene,const std::string& label );

    std::vector<Ref<FloatKey>> getKeys() override {
        return m_keys;
    }
    
    virtual Ref<FloatKey> getKeyFromIdx(const unsigned int idx) override;  
    virtual Ref<FloatKey> getKeyFromTime(const Time& t) override;

    virtual void addKey(Ref<FloatKey> key) override { m_keys.push_back(key);m_dirty=true; };

    unsigned int getPreviousKeyIdx(const Time& t) const;

    float virtual eval(const Time& t) override ;
    virtual void setValAtTime(const Time& t, float val)override;
    virtual void dirty() override { m_dirty = true; }
};

class XformController {
public:    
    enum class XformControllerType{
        Euler = 0,
        Quaternion = 1,
        Script = 2
    };

    XformController(XformControllerType t, std::weak_ptr<Scene> scene,const std::string& label="No Name");

    virtual XformControllerType getType() const  { return m_type; }

    virtual glm::mat4 eval(const Time& t) =0;
    virtual void setFromMat4(const glm::mat4& xform, Time& t) = 0;

    virtual glm::vec3 evalPosition(const Time& t) const;
    virtual glm::vec3 evalEulerAngles(const Time& t) const = 0;
    virtual glm::vec3 evalScale(const Time& t) const;
    // this will set the key at the time, or create one
    virtual void setPositionAtTime(const Time& t, const glm::vec3& p);
    virtual void setEulerAnglesAtTime(const Time& t, const glm::vec3& r)  = 0;
    virtual void setScaleAtTime(const Time& t, const glm::vec3& s);

    Ref<FloatController>& getXPosController() { return m_xPosController; }
    Ref<FloatController>& getYPosController() { return m_yPosController; }
    Ref<FloatController>& getZPosController() { return m_zPosController; }

    Ref<FloatController>& getXScaleController() { return m_xScaleController; }
    Ref<FloatController>& getYScaleController() { return m_yScaleController; }
    Ref<FloatController>& getZScaleController() { return m_zScaleController; }

    virtual void dirty();

protected:
    uint32_t m_id;
    XformControllerType m_type;
    std::string m_label;
    glm::mat4 m_cache;
    bool m_dirty;
    std::weak_ptr<Scene> m_scene;

    Ref<FloatController> m_xPosController  ;
    Ref<FloatController> m_yPosController  ;
    Ref<FloatController> m_zPosController  ;

    Ref<FloatController> m_xScaleController;
    Ref<FloatController> m_yScaleController;
    Ref<FloatController> m_zScaleController;

};

class EulerXformController : public XformController{
public:
    EulerXformController(std::weak_ptr<Scene> scene,const std::string& label="No Name" );

    virtual glm::mat4 eval(const Time& t) override;
    virtual void setFromMat4(const glm::mat4& xform, Time& t) override;

    virtual glm::vec3 evalEulerAngles(const Time& t) const override;

    // this will set the key at the time, or create one
    virtual void setEulerAnglesAtTime(const Time& t, const glm::vec3& r) override;


    Ref<FloatController>& getXAngleController() { return m_xAngleController; }
    Ref<FloatController>& getYAngleController() { return m_yAngleController; }
    Ref<FloatController>& getZAngleController() { return m_zAngleController; }

    virtual void dirty() override;

private:
    Ref<FloatController> m_xAngleController  ; 
    Ref<FloatController> m_yAngleController  ; 
    Ref<FloatController> m_zAngleController  ; 
};

// class QuaternionXformController : public XformController{
//     
// };
}
