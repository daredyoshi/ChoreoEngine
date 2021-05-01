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
        Ref<FloatStepController> getPerspectiveFOV() { return m_perspectiveFOV; }
        void setPerspectiveFOV(float val, const Scope<Time>& t){ m_perspectiveFOV->setVal(val, t); recalculateProjection(t); }
        Ref<FloatStepController> getPerspectiveNearClip() { return m_perspectiveNearClip; }
        void setPerspectiveNearClip(float val, const Scope<Time>& t){ m_perspectiveNearClip->setVal(val, t); recalculateProjection(t); }
        Ref<FloatStepController> getPerspectiveFarClip() { return m_perspectiveFarClip; }
        void setPerspectiveFarClip(float val, const Scope<Time>& t){ m_perspectiveFarClip->setVal(val, t); recalculateProjection(t); }
        
        // ORTHOGRAPHIC
        Ref<FloatStepController> getOrthographicSize() { return m_orthographicSize; }
        void setOrthographicSize(float val, const Scope<Time>& t){ m_orthographicSize->setVal(val, t); recalculateProjection(t); }
        // void setPerspectiveFOV(float val){ m_perspectiveFOV->setVal(val); recalculateProjection(); }
        Ref<FloatStepController> getOrthographicNearClip() { return m_orthographicNearClip; }
        void setOrthographicNearClip(float val, const Scope<Time>& t){ m_orthographicNearClip->setVal(val, t); recalculateProjection(t); }
        Ref<FloatStepController> getOrthographicFarClip() { return m_orthographicFarClip; }
        void setOrthographicFarClip(float val, const Scope<Time>& t){ m_orthographicFarClip->setVal(val, t); recalculateProjection(t); }


        float getAspectRatio() const { return m_aspectRatio; }
        
        ProjectionType getProjectionType() const { return m_projectionType; }
        void setProjectionType(ProjectionType type) { m_projectionType = type; recalculateProjection(); }

    private:
        void recalculateProjection(const Scope<Time>& t);
        void recalculateProjection();
        ProjectionType m_projectionType = ProjectionType::Perspective;
        Ref<FloatStepController> m_orthographicSize {CreateRef<FloatStepController>(10.0f)};
        Ref<FloatStepController> m_orthographicNearClip {CreateRef<FloatStepController>(-1.0f)};
        Ref<FloatStepController> m_orthographicFarClip {CreateRef<FloatStepController>(1.0f)};

        Ref<FloatStepController> m_perspectiveFOV{CreateRef<FloatStepController>(glm::radians(45.0f))};
        Ref<FloatStepController> m_perspectiveNearClip {CreateRef<FloatStepController>(0.001f)};
        Ref<FloatStepController> m_perspectiveFarClip {CreateRef<FloatStepController>(1000.0f)};

        float m_aspectRatio{0};
        Time t_cache{Time{0}};
    };
}

