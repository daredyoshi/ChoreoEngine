#pragma once


#include "Application/Renderer/Camera.h"
#include "glm/glm.hpp"

namespace ChoreoApp {
    class SceneCamera : public Camera{
    public:
        enum class ProjectionType { Perspective = 0, Orthographic = 1 };


        SceneCamera();
        virtual ~SceneCamera() override {};

        void setViewportSize(const uint32_t width,const uint32_t height);

        // PERSPECTIVE 
        void setPerspective(float FOV, float nearClip, float farClip);

        float getPerspectiveVerticalFOV() const { return m_perspectiveFOV; }
        void setPerspectiveVerticalFOV(float perspectiveFOV){ m_perspectiveFOV= perspectiveFOV; recalculateProjection();}

        float getPerspectiveNearClip() const { return m_perspectiveNear; }
        void setPerspectiveNearClip(float nearClip) { m_perspectiveNear= nearClip;  recalculateProjection();}

        float getPerspectiveFarClip() const { return m_perspectiveFar; }
        void setPerspectiveFarClip(float farClip) { m_perspectiveFar= farClip; recalculateProjection(); }
        
        // ORTHOGRAPHIC
        void setOrthographic(float size, float nearClip, float farClip);

        float getOrthographicSize() const { return m_orthographicSize; }
        void setOrthographicSize(float orthographicSize){ m_orthographicSize = orthographicSize; recalculateProjection();}

        float getOrthographicNearClip() const { return m_orthographicNear; }
        void setOrthographicNearClip(float nearClip) { m_orthographicNear= nearClip;  recalculateProjection();}

        float getOrthographicFarClip() const { return m_orthographicFar; }
        void setOrthographicFarClip(float farClip) { m_orthographicFar= farClip; recalculateProjection(); }

        float getAspectRatio() const { return m_aspectRatio; }
        
        ProjectionType getProjectionType() const { return m_projectionType; }
        void setProjectionType(ProjectionType type) { m_projectionType = type; recalculateProjection(); }

    private:
        ProjectionType m_projectionType = ProjectionType::Perspective;
        void recalculateProjection();
        float m_orthographicSize = 10.0f;
        float m_orthographicNear = -1.0f;
        float m_orthographicFar = 1.0f;

        float m_perspectiveFOV = glm::radians(45.0f);
        float m_perspectiveNear = 0.01f;
        float m_perspectiveFar = 1000.0f;

        float m_aspectRatio{0};
    };
}

