#include "Controller.h"
#include "UniqueID.h"
#include "Scene.h"
#include "Entity.h"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include <memory>

namespace ChoreoApp{

    FloatController::FloatController(ControllerType t, const std::string& label)
        : m_type{t}, m_label{label} {

        // m_id = scene.lock()->getID();
        m_id = UniqueID().id;
    }

    void FloatController::addOnDirtyCallback(std::string& callbackName, std::function<void()> callback){
        if(m_onDirtyCallbacks.find(callbackName) == m_onDirtyCallbacks.end()){
            m_onDirtyCallbacks[callbackName] = callback;
        }
        else{
            CE_WARN("Callback key already exists new callback has not been added!");
        }
    }

    void FloatController::dirty(){
        for (auto& fn : m_onDirtyCallbacks){
            fn.second();
        }
    }

    StaticFloatController::StaticFloatController(const std::string& label)
        : FloatController(FloatController::ControllerType::Static, label) {}

    StaticFloatController::StaticFloatController(float val, const std::string& label)
        : FloatController(FloatController::ControllerType::Static, label) {
            m_key->setVal(val);
            setTicksPerSample(1000000);
    }

    AnimatedFloatController::AnimatedFloatController(const std::string& label, Time& startTime, Time& endTime )
        : FloatController(FloatController::ControllerType::Animated, label), m_startTime{startTime}, m_endTime{endTime} {
    }

    void AnimatedFloatController::dirty(){
        FloatController::dirty();
        // this must come last because the diry callbacks
        // might undirty the controller
        m_dirty = true;
    }

    void AnimatedFloatController::assertAtLeastOneKeyExists(){
        // there must always be at least 1 key
        if (!this->m_keys.size()){
            m_keys.push_back(CreateRef<FloatKey>( 0.0f ));
        }

    }


    void AnimatedFloatController::cacheTimeRange(){
        m_cache.resize(( m_endTime.getTick() - m_startTime.getTick() ) / getTicksPerSample()); 

        for(uint32_t tickIdx{0}; tickIdx<m_endTime.getTick()/ getTicksPerSample(); ++tickIdx){
            Ref<FloatKey> k = getPreviousKey(tickIdx * getTicksPerSample());
            if(k->getToNextKeyInterpolationType() == FloatKey::KeyInterpolationType::Static){
                m_cache[tickIdx] = k->eval();
            }
        }
    }

    void AnimatedFloatController::addOnSetValDirtyControllerCallback(Ref<FloatKey> key){
        std::string name{"CA_dirtyParentController"};
        key->addOnSetValCallback(name, 
                std::function<void()>(std::bind(&AnimatedFloatController::dirty, this))
        );

    }

    void AnimatedFloatController::addKey(Ref<FloatKey> key){
        addOnSetValDirtyControllerCallback(key);
        m_keys.push_back(key);
        m_dirty=true;
    }

    void AnimatedFloatController::swapKeys(uint32_t idxA, uint32_t idxB){
        std::swap(m_keys[idxA], m_keys[idxB]);
    }

    Ref<FloatKey> AnimatedFloatController::getPreviousKey(const Time& t) const {
        return getPreviousKey(t.getTick());
    } 
    Ref<FloatKey> AnimatedFloatController::getPreviousKey(uint32_t tick) const {
        return m_keys[getPreviousKeyIdx(tick)];
    }

    unsigned int AnimatedFloatController::getPreviousKeyIdx(const Time& t) const {
        return getPreviousKeyIdx(t.getTick());
    } 
    unsigned int AnimatedFloatController::getPreviousKeyIdx(uint32_t tick) const {
        CE_CORE_ASSERT(m_keys.size() > 0, "There must ALWAYS be at least one key");
        // check to see if this is single value
        if ( this->m_keys.size() == 1){
            return 0;
        }

        for(uint32_t i = 1; i < this->m_keys.size(); ++i){
            Ref<FloatKey> k = this->m_keys[i]; 
            // convert the key to ticks and the time to tcks
            if (k->getTick() >= tick){
                return i-1;
            }
        }
        
        // if no keys are greater than T return the final key
        return m_keys.size()-1;
    }

    float AnimatedFloatController::eval(const Time& t) {
        if(m_dirty){
            cacheTimeRange();
            m_dirty=false;
        }

        // it's fine to use int division
        return m_cache[t.getTick() / getTicksPerSample()];
    }       

    void AnimatedFloatController::setValAtTime(const Time& t, float val){
        Ref<FloatKey> k = getPreviousKey(t);
        if(k){
            k->setVal(val);
        }
        m_dirty=true;
    }

    XformController::XformController(XformControllerType t, const std::string& label)
        : m_type{t}, m_label{label} {


        m_id = UniqueID().id;

        m_xPosController = std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>(0.0f, "X Position"));     
        m_yPosController = std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>(0.0f, "Y Position"));     
        m_zPosController = std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>(0.0f, "Z Position"));     
                                                                                                                  
        m_xScaleController = std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>( 1.0f, "X Scale"));   
        m_yScaleController = std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>( 1.0f, "Y Scale"));   
        m_zScaleController = std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>( 1.0f, "Z Scale"));   
    }

    void XformController::dirty() {
        m_xPosController->dirty();
        m_yPosController->dirty();
        m_zPosController->dirty();

        m_xScaleController->dirty();
        m_yScaleController->dirty();
        m_zScaleController->dirty();
        m_dirty = true;
    }

    glm::vec3 XformController::evalPosition(const Time& t) const{
        return {
                m_xPosController->eval(t),
                m_yPosController->eval(t),
                m_zPosController->eval(t)
        };   
    }
    glm::vec3 XformController::evalScale(const Time& t) const{
        return {
                m_xScaleController->eval(t),
                m_yScaleController->eval(t),
                m_zScaleController->eval(t)
        };   
    }

    void XformController::setPositionAtTime(const Time& t, const glm::vec3& p){
        m_xPosController->setValAtTime(t, p.x);
        m_yPosController->setValAtTime(t, p.y);
        m_zPosController->setValAtTime(t, p.z);
        m_dirty = true;
    }
    void XformController::setScaleAtTime(const Time& t, const glm::vec3& s){
        m_xScaleController->setValAtTime(t, s.x);
        m_yScaleController->setValAtTime(t, s.y);
        m_zScaleController->setValAtTime(t, s.z);
        m_dirty = true;
    }

    EulerXformController::EulerXformController(const std::string& label)
        : XformController(XformController::XformControllerType::Euler, label) {

        m_xAngleController = std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>( 0.0f, "X Angle"));
        m_yAngleController = std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>( 0.0f, "Y Angle"));
        m_zAngleController = std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>( 0.0f, "Z Angle"));
    }

    glm::mat4 EulerXformController::eval(const Time& t) {
        if(m_dirty){
            glm::vec3 p {
                m_xPosController->eval(t),
                m_yPosController->eval(t),
                m_zPosController->eval(t)
            };   
            glm::vec3 r {
                m_xAngleController->eval(t),
                m_yAngleController->eval(t),
                m_zAngleController->eval(t)
            };
            glm::vec3 s{
                m_xScaleController->eval(t),
                m_yScaleController->eval(t),
                m_zScaleController->eval(t)
            };

            glm::mat4 translation = glm::translate(p);
            glm::mat4 rotation = glm::eulerAngleXYZ( glm::radians(r.x), glm::radians(r.y),glm::radians( r.z ));
            glm::mat4 scale = glm::scale(s);

            m_cache =translation * rotation * scale;
            m_dirty = false;
        }
        return m_cache;
    }

    void EulerXformController::dirty(){
        m_xAngleController->dirty();
        m_yAngleController->dirty();
        m_zAngleController->dirty();

        XformController::dirty();
    }

    glm::vec3 EulerXformController::evalEulerAngles(const Time& t) const{
        return {
                m_xAngleController->eval(t),
                m_yAngleController->eval(t),
                m_zAngleController->eval(t)
        };   
    }
    void EulerXformController::setEulerAnglesAtTime(const Time& t, const glm::vec3& p){
        m_xAngleController->setValAtTime(t, p.x);
        m_yAngleController->setValAtTime(t, p.y);
        m_zAngleController->setValAtTime(t, p.z);
        m_dirty = true;
    }

    void EulerXformController::setFromMat4(const glm::mat4& xform, Time& t){
        glm::vec3 s;
        glm::quat q;
        glm::vec3 p;
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(xform, s, q, p, skew, perspective);
        glm::vec3 r = glm::eulerAngles(q);

        m_xPosController->setValAtTime(t, p.x);
        m_yPosController->setValAtTime(t, p.y);
        m_zPosController->setValAtTime(t, p.z);
        
        m_xAngleController->setValAtTime(t, r.x);
        m_yAngleController->setValAtTime(t, r.y);
        m_zAngleController->setValAtTime(t, r.z);

        m_xScaleController->setValAtTime(t, s.x);
        m_yScaleController->setValAtTime(t, s.y);
        m_zScaleController->setValAtTime(t, s.z);
        // since the values have changed 
        m_dirty = true;
    }
}
