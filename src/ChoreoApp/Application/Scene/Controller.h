#pragma once
#include "Application/Scene/Scene.h"
#include "capch.h"
#include "Key.h"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include <memory>
#include <unordered_map>

namespace ChoreoApp{


class Scene;

class FloatController{

public:
    enum class ControllerType{
        Static = 0,
        Animated = 1,
        Script = 2
    };
    FloatController(ControllerType t, const std::string& label="No Name");

    const std::string& getLabel() const { return m_label; }
    void setLabel(const std::string& label) { m_label = label; }
    virtual uint32_t getID() const { return m_id; }
    virtual ControllerType getType() const  { return m_type; }

    // these are the pure virtuals 
    float virtual eval(const Time& t) = 0;

    virtual std::vector<Ref<FloatKey>> getKeys() =0;

    // this will get a key at time t if it exists 
    virtual Ref<FloatKey> getPreviousKey(const Time& t) const=0;
    virtual uint32_t getPreviousKeyIdx(const Time& t) const =0;
    virtual void swapKeys(uint32_t idxA, uint32_t idxB) =0;
    virtual void addKey(Ref<FloatKey> key) =0;
    virtual void removeKeyFromIdx(const unsigned int idx) = 0;
    virtual Ref<FloatKey> getKeyFromIdx(const uint32_t idx) const=0;

    // if a key exists at time t, this will set it, otherwise
    // it will only set the cached value which will be cleared on
    // slidertime change
    virtual void setValAtTime(const Time& t, float val) = 0;

    // this will force update the cache
    virtual void dirty();

    virtual void addOnDirtyCallback(std::string& callbackName, std::function<void()> callback);

private:
    ControllerType m_type;
    std::string m_label{};
    uint32_t m_id;
    // these are called whenever the controller is diried
    std::unordered_map<std::string, std::function<void()>> m_onDirtyCallbacks {};
};

class StaticFloatController : public FloatController{ 
public:
    StaticFloatController(const std::string& label="No Name");
    StaticFloatController(float val, const std::string& label="No Name" );

    float virtual eval(const Time& t) override {(void)t; return this->m_key->eval(); }       

    virtual std::vector<Ref<FloatKey>> getKeys() override { return {{ m_key }}; };


    virtual Ref<FloatKey> getPreviousKey(const Time& t)const override { (void)t;return m_key; }
    virtual uint32_t getPreviousKeyIdx(const Time& t)const override { (void)t;return 0; }
    virtual void swapKeys(uint32_t idxA, uint32_t idxB) override {(void)idxA; (void)idxB;};
    virtual void addKey(Ref<FloatKey> key)override { m_key = key; };
    virtual void removeKeyFromIdx(unsigned int idx) override {  (void)idx;} ;
    virtual Ref<FloatKey> getKeyFromIdx(uint32_t idx) const override { (void) idx; return m_key; }

    virtual void setValAtTime(const Time& t, float val) override{(void)t; this->m_key->setVal(val);}

    virtual void dirty() override {};

protected:
    Ref<FloatKey> m_key{CreateRef<FloatKey>()};
};


class AnimatedFloatController : public FloatController{
protected:
    std::vector<Ref<FloatKey>> m_keys;
    int m_ticksPerSample{30}; // TODO update this to default to the ticks per frame with a marco 
    Time& m_startTime;
    Time& m_endTime;
    std::vector<float> m_cache{0.0f};
    bool m_dirty{true};

public:
    AnimatedFloatController(const std::string& label, Time& startTime, Time& endTime );

    float virtual eval(const Time& t) override ;

    std::vector<Ref<FloatKey>> getKeys() override { return m_keys;}
    
    Ref<FloatKey> getPreviousKey(const Time& t) const override;
    unsigned int getPreviousKeyIdx(const Time& t) const override;
    virtual void swapKeys(uint32_t idxA, uint32_t idxB) override;
    virtual void addKey(Ref<FloatKey> key) override { m_keys.push_back(key);m_dirty=true; };
    virtual void removeKeyFromIdx(const unsigned int idx) override {  m_keys.erase(m_keys.begin() + idx);} ;
    virtual Ref<FloatKey> getKeyFromIdx(const uint32_t idx) const override { return m_keys[idx]; }

    virtual void setValAtTime(const Time& t, float val)override;
    virtual void dirty() override;
private:
    void cacheTimeRange();
    void assertAtLeastOneKeyExists();
};

class XformController {
public:    
    enum class XformControllerType{
        Euler = 0,
        Quaternion = 1,
        Script = 2
    };

    XformController(XformControllerType t, const std::string& label="No Name");

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

    Ref<FloatController> m_xPosController  ;
    Ref<FloatController> m_yPosController  ;
    Ref<FloatController> m_zPosController  ;

    Ref<FloatController> m_xScaleController;
    Ref<FloatController> m_yScaleController;
    Ref<FloatController> m_zScaleController;

};

class EulerXformController : public XformController{
public:
    EulerXformController(const std::string& label="No Name" );

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
