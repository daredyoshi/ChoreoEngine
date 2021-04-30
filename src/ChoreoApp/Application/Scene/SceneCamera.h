#pragma once


#include "Application/Renderer/Camera.h"
#include "Controller.h"
#include "glm/glm.hpp"

namespace ChoreoApp {
    class SceneCamera : public Camera{
    public:
        enum class ProjectionType { Perspective = 0, Orthographic = 1 };


        SceneCamera();
        virtual ~SceneCamera() override {};

        void setViewportSize(const uint32_t width,const uint32_t height);

        // PERSPECTIVE 
        Ref<FloatController> getPerspectiveFOV() { return m_perspectiveFOV; }
        void setPerspectiveFOV(double val){ m_perspectiveFOV->setVal(val); recalculateProjection(); }
        Ref<FloatController> getPerspectiveNearClip() { return m_perspectiveNearClip; }
        void setPerspectiveNearClip(double val){ m_perspectiveNearClip->setVal(val); recalculateProjection(); }
        Ref<FloatController> getPerspectiveFarClip() { return m_perspectiveFarClip; }
        void setPerspectiveFarClip(double val){ m_perspectiveFarClip->setVal(val); recalculateProjection(); }
        
        // ORTHOGRAPHIC
        Ref<FloatController> getOrthographicSize() { return m_orthographicSize; }
        void setPerspectiveFOV(double val){ m_perspectiveFOV->setVal(val); recalculateProjection(); }
        Ref<FloatController> getOrthographicNearClip() { return m_orthographicNearClip; }
        Ref<FloatController> getOrthographicFarClip() { return m_orthographicFarClip; }


        float getAspectRatio() const { return m_aspectRatio; }
        
        ProjectionType getProjectionType() const { return m_projectionType; }
        void setProjectionType(ProjectionType type) { m_projectionType = type; recalculateProjection(); }

    private:
        ProjectionType m_projectionType = ProjectionType::Perspective;
        void recalculateProjection();
        Ref<FloatController> m_orthographicSize {CreateRef<FloatController>(10.0f)};
        Ref<FloatController> m_orthographicNearClip {CreateRef<FloatController>(-1.0f)};
        Ref<FloatController> m_orthographicFarClip {CreateRef<FloatController>(1.0f)};

        Ref<FloatController> m_perspectiveFOV{CreateRef<FloatController>(glm::radians(45.0f))};
        Ref<FloatController> m_perspectiveNearClip {CreateRef<FloatController>(0.001f)};
        Ref<FloatController> m_perspectiveFarClip {CreateRef<FloatController>(1000.0f)};

        float m_aspectRatio{0};
    };
}

