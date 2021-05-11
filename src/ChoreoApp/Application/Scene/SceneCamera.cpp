
#include "capch.h"
#include "SceneCamera.h"
#include "glm/ext/matrix_clip_space.hpp"
#include <glm/glm.hpp>

namespace ChoreoApp {
    SceneCamera::SceneCamera(std::weak_ptr<Scene> scene) 
        : Camera() {
        m_scene = scene; 

        recalculateProjection(CreateScope<Time>(t_cache));

        m_orthographicSize = std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>(m_scene, 10.0f));
        m_orthographicNearClip = std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>(m_scene, -1.0f));
        m_orthographicFarClip = std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>(m_scene, 1.0f));
                                                                                                             
        m_perspectiveFOV = std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>(m_scene, glm::radians(45.0f)));
        m_perspectiveNearClip = std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>(m_scene, 0.001f));
        m_perspectiveFarClip = std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>(m_scene, 1000.0f));
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

    void SceneCamera::recalculateProjection(const Scope<Time>& t){
        // create and cache a copy of time or faster processing
        t_cache = *t;
        recalculateProjection();
    }
    void SceneCamera::recalculateProjection(){
        Scope<Time> t_cachePtr = CreateScope<Time>(t_cache);
        if (m_projectionType == ProjectionType::Perspective){
            m_projectionMatrix = glm::perspective(m_perspectiveFOV->eval(t_cachePtr), m_aspectRatio, m_perspectiveNearClip->eval(t_cachePtr), m_perspectiveFarClip->eval(t_cachePtr));
        }
        else{
            float orthoLeft = -m_orthographicSize->eval(t_cachePtr) * m_aspectRatio * 0.5f ;
            float orthoRight= m_orthographicSize->eval(t_cachePtr) * m_aspectRatio * 0.5f ;
            float orthoTop= m_orthographicSize->eval(t_cachePtr) * 0.5f ;
            float orthoBottom= -m_orthographicSize->eval(t_cachePtr) * 0.5f ;
            m_projectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop,
                    m_orthographicNearClip->eval(t_cachePtr), m_orthographicFarClip->eval(t_cachePtr));

        }
    }

    void SceneCamera::dirty(){
        m_orthographicSize->dirty();
        m_orthographicNearClip->dirty();
        m_orthographicFarClip->dirty();

        m_perspectiveFOV->dirty();
        m_perspectiveNearClip->dirty();
        m_perspectiveFarClip->dirty();
    }
}
