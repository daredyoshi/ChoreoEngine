#include "Controller.h"
#include "Scene.h"
#include "Entity.h"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/matrix_decompose.hpp"

namespace ChoreoApp{


    void XformController::dirty() {
        m_xPosController->dirty();
        m_yPosController->dirty();
        m_zPosController->dirty();

        m_xScaleController->dirty();
        m_yScaleController->dirty();
        m_zScaleController->dirty();
        m_dirty = true;
    }

    glm::vec3 XformController::evalPosition(const Scope<Time>& t) const{
        return {
                m_xPosController->eval(t),
                m_yPosController->eval(t),
                m_zPosController->eval(t)
        };   
    }
    glm::vec3 XformController::evalScale(const Scope<Time>& t) const{
        return {
                m_xScaleController->eval(t),
                m_yScaleController->eval(t),
                m_zScaleController->eval(t)
        };   
    }

    void XformController::setPositionAtTime(const Scope<Time>& t, const glm::vec3& p){
        m_xPosController->setValAtTime(t, p.x);
        m_yPosController->setValAtTime(t, p.y);
        m_zPosController->setValAtTime(t, p.z);
        m_dirty = true;
    }
    void XformController::setScaleAtTime(const Scope<Time>& t, const glm::vec3& s){
        m_xScaleController->setValAtTime(t, s.x);
        m_yScaleController->setValAtTime(t, s.y);
        m_zScaleController->setValAtTime(t, s.z);
        m_dirty = true;
    }

    glm::mat4 EulerXformController::eval(const Scope<Time>& t) {
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

    glm::vec3 EulerXformController::evalEulerAngles(const Scope<Time>& t) const{
        return {
                m_xAngleController->eval(t),
                m_yAngleController->eval(t),
                m_zAngleController->eval(t)
        };   
    }
    void EulerXformController::setEulerAnglesAtTime(const Scope<Time>& t, const glm::vec3& p){
        m_xAngleController->setValAtTime(t, p.x);
        m_yAngleController->setValAtTime(t, p.y);
        m_zAngleController->setValAtTime(t, p.z);
        m_dirty = true;
    }

    void EulerXformController::setFromMat4(const glm::mat4& xform, Scope<Time>& t){
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
