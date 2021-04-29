
#include "capch.h"
#include "SceneCamera.h"
#include "glm/ext/matrix_clip_space.hpp"
#include <glm/glm.hpp>

namespace ChoreoApp {
    SceneCamera::SceneCamera() 
        : Camera() {
        recalculateProjection();
    }

    void SceneCamera::setOrthographic(float size, float nearClip, float farClip){
        m_projectionType = ProjectionType::Orthographic;
        m_orthographicSize = size;
        m_orthographicNear = nearClip;
        m_orthographicFar = farClip;
        recalculateProjection();
    }
    void SceneCamera::setPerspective(float fov, float nearClip, float farClip){
        m_projectionType = ProjectionType::Perspective;
        m_perspectiveFOV = fov;
        m_perspectiveNear = nearClip;
        m_perspectiveFar = farClip;
        recalculateProjection();
    }

    void SceneCamera::setViewportSize(const uint32_t width, const uint32_t height){
        m_aspectRatio = (float)width / (float)height;
        recalculateProjection();
    }
    void SceneCamera::recalculateProjection(){
        if (m_projectionType == ProjectionType::Perspective){
            m_projectionMatrix = glm::perspective(m_perspectiveFOV, m_aspectRatio, m_perspectiveNear, m_perspectiveFar);
        }
        else{
            float orthoLeft = -m_orthographicSize * m_aspectRatio * 0.5f ;
            float orthoRight= m_orthographicSize * m_aspectRatio * 0.5f ;
            float orthoTop= m_orthographicSize * 0.5f ;
            float orthoBottom= -m_orthographicSize * 0.5f ;
            m_projectionMatrix = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop,
                    m_orthographicNear, m_orthographicFar);

        }
    }
}
