#include "Controller.h"
#include "Scene.h"
#include "Entity.h"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/matrix_decompose.hpp"

namespace ChoreoApp{

    glm::mat4 EulerXformController::eval(const Scope<Time>& t) const{
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
        return translation * rotation * scale;
    }

    void EulerXformController::setFromMat4(const glm::mat4& xform, Scope<Time>& t){
        glm::vec3 s;
        glm::quat q;
        glm::vec3 p;
        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(xform, s, q, p, skew, perspective);
        glm::vec3 r = glm::eulerAngles(q);

        m_xPosController->setVal(t, p.x);
        m_yPosController->setVal(t, p.y);
        m_zPosController->setVal(t, p.z);
        
        m_xAngleController->setVal(t, r.x);
        m_yAngleController->setVal(t, r.y);
        m_zAngleController->setVal(t, r.z);

        m_xScaleController->setVal(t, s.x);
        m_yScaleController->setVal(t, s.y);
        m_zScaleController->setVal(t, s.z);
    }
}
