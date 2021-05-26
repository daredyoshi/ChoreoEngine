
#include "capch.h"
#include "Entity.h"
#include "SceneCamera.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/trigonometric.hpp"
#include <glm/glm.hpp>

namespace ChoreoApp {
    SceneCamera::SceneCamera() 
        : Camera() {
        m_orthographicSize = std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>( 10.0f, "Ortho Size"));
        m_orthographicNearClip = std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>( -1.0f, "Near Clip"));
        m_orthographicFarClip = std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>( 1.0f, "Far Clip"));
                                                                                                             
        m_perspectiveFOV = std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>( glm::radians(45.0f), "FOV"));
        m_perspectiveNearClip = std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>( 0.001f, "Near Clip"));
        m_perspectiveFarClip = std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>( 1000.0f, "Far Clipe"));

        recalculateProjection();
    }

    // void SceneCamera::setOrthographic(float size, float nearClip, float farClip){
    //     m_projectionType = ProjectionType::Orthographic;
    //     m_orthographicSize = size;
    //     m_orthographicNear = nearClip;
    //     m_orthographicFar = farClip;
    //     recalculateProjection();
    // }
    // void SceneCamera::setPerspective(float fov, float nearClip, float farClip){
    //     m_projectionType = ProjectionType::Perspective;
    //     m_perspectiveFOV = fov;
    //     m_perspectiveNear = nearClip;
    //     m_perspectiveFar = farClip;
    //     recalculateProjection();
    // }

    void SceneCamera::setViewportSize(const uint32_t width, const uint32_t height){
        m_aspectRatio = (float)width / (float)height;
        recalculateProjection();
    }

    void SceneCamera::recalculateProjection(){
        if (m_projectionType == ProjectionType::Perspective){
            m_projectionMatrix = glm::perspective(glm::radians(m_perspectiveFOV->eval(t_cache)), 
            m_aspectRatio, m_perspectiveNearClip->eval(t_cache), 
            m_perspectiveFarClip->eval(t_cache));
        }
        else{
            float orthoLeft = -m_orthographicSize->eval(t_cache) * m_aspectRatio * 0.5f ;
            float orthoRight= m_orthographicSize->eval(t_cache) * m_aspectRatio * 0.5f ;
            float orthoTop= m_orthographicSize->eval(t_cache) * 0.5f ;
            float orthoBottom= -m_orthographicSize->eval(t_cache) * 0.5f ;
            m_projectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop,
                    m_orthographicNearClip->eval(t_cache), m_orthographicFarClip->eval(t_cache));

        }
    }

    void SceneCamera::addOnDirtyRecalculateProjectionCallback(Ref<FloatController> controller){
        std::string name{"CA_dirtyParentController"};
        controller->addOnDirtyCallback(name, 
                std::function<void()>(std::bind(&SceneCamera::update, this))
        );
    }

    void SceneCamera::setTime(Time& t){
        t_cache = t;
        recalculateProjection();
    }

    void SceneCamera::update(){
        recalculateProjection();
    }
}
