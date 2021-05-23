#include "Controller.h"
#include "UniqueID.h"
#include "Scene.h"
#include "Entity.h"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include <memory>

namespace ChoreoApp{

    FloatController::FloatController(ControllerType t, std::weak_ptr<Scene> scene, const std::string& label)
        : m_type{t}, m_scene{scene}, m_label{label} {

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

    StaticFloatController::StaticFloatController(std::weak_ptr<Scene> scene, const std::string& label)
        : FloatController(FloatController::ControllerType::Static, scene, label) {}

    StaticFloatController::StaticFloatController(std::weak_ptr<Scene> scene, float val, const std::string& label)
        : FloatController(FloatController::ControllerType::Static, scene, label) {
            m_key->setVal(val);
    }

    AnimatedFloatController::AnimatedFloatController(std::weak_ptr<Scene> scene,const std::string& label )
        : FloatController(FloatController::ControllerType::Animated, scene, label) {
        m_keys.push_back(CreateRef<FloatKey>());            
    }

    void AnimatedFloatController::dirty(){
        FloatController::dirty();
        // this must come last because the diry callbacks
        // might undirty the controller
        m_dirty = true;
    }

    Ref<FloatKey> AnimatedFloatController::getKeyFromIdx(const unsigned int idx) {
        return this->m_keys[idx]; 
    }

    Ref<FloatKey> AnimatedFloatController::getKeyFromTime(const Time& t) {
        
        // check to see if this is single value
        if ( this->m_keys.size() == 1){
            return m_keys[0];
        }

        for(unsigned int i = 0; i < this->m_keys.size(); ++i){
            Ref<FloatKey> k = this->m_keys[i]; 

            if (k->getTick() >= t.getTick()){
                return k;
            }
            // if (k->getTick() > t->getTick()){
            //     return nullptr;
            // }
        }
        CE_CORE_ASSERT(false, "Controller did not find keys")
        return m_keys[0];
    }

    void AnimatedFloatController::swapKeys(uint32_t idxA, uint32_t idxB){
        std::swap(m_keys[idxA], m_keys[idxB]);
    }

    unsigned int AnimatedFloatController::getPreviousKeyIdx(const Time& t) const {
        CE_CORE_ASSERT(m_keys.size() > 0, "There must ALWAYS be at least one key");
        // check to see if this is single value
        if ( this->m_keys.size() == 1){
            return 0;
        }

        for(uint32_t i = 1; i < this->m_keys.size(); ++i){
            Ref<FloatKey> k = this->m_keys[i]; 
            // convert the key to ticks and the time to tcks
            if (k->getTick() >= t.getTick()){
                return i-1;
            }
        }
        
        // if no keys are greater than T return the final key
        return m_keys.size()-1;
    }

    float AnimatedFloatController::eval(const Time& t) {
        if(m_dirty){
            if(m_keys.size() ==0){
                return float{};
            }
            unsigned int idx = getPreviousKeyIdx(t);

            // default to stepped interpolation
            m_cache = m_keys[idx]->eval();
            m_dirty = false;
        } 
        return m_cache; 
    }       
    void AnimatedFloatController::setValAtTime(const Time& t, float val){
        unsigned int idx = getPreviousKeyIdx(t);
        Ref<FloatKey> k = getKeyFromTime(t);
        if(k){
            k->setVal(val);
        }
        m_dirty=true;
    }

    XformController::XformController(XformControllerType t, std::weak_ptr<Scene> scene, const std::string& label)
        : m_type{t}, m_label{label} {

        m_scene = scene; 

        m_id = m_scene.lock()->getID();

        m_xPosController = std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>(m_scene, 0.0f, "X Position"));     
        m_yPosController = std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>(m_scene, 0.0f, "Y Position"));     
        m_zPosController = std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>(m_scene, 0.0f, "Z Position"));     
                                                                                                                  
        m_xScaleController = std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>(m_scene, 1.0f, "X Scale"));   
        m_yScaleController = std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>(m_scene, 1.0f, "Y Scale"));   
        m_zScaleController = std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>(m_scene, 1.0f, "Z Scale"));   
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

    EulerXformController::EulerXformController(std::weak_ptr<Scene> scene, const std::string& label)
        : XformController(XformController::XformControllerType::Euler, scene, label) {

        m_xAngleController = std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>(m_scene, 0.0f, "X Angle"));
        m_yAngleController = std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>(m_scene, 0.0f, "Y Angle"));
        m_zAngleController = std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>(m_scene, 0.0f, "Z Angle"));
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
