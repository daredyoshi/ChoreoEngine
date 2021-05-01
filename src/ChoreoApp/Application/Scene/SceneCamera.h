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
        Ref<StepController<1>> getPerspectiveFOV() { return m_perspectiveFOV; }
        void setPerspectiveFOV(float val, const Scope<Time>& t){ m_perspectiveFOV->setVal(std::array<float, 1>{{val}}, t); recalculateProjection(); }
        Ref<StepController<1>> getPerspectiveNearClip() { return m_perspectiveNearClip; }
        void setPerspectiveNearClip(float val, const Scope<Time>& t){ m_perspectiveNearClip->setVal(std::array<float, 1>{{val}}, t); recalculateProjection(); }
        Ref<StepController<1>> getPerspectiveFarClip() { return m_perspectiveFarClip; }
        void setPerspectiveFarClip(float val, const Scope<Time>& t){ m_perspectiveFarClip->setVal(std::array<float, 1>{{val}}, t); recalculateProjection(); }
        
        // ORTHOGRAPHIC
        Ref<StepController<1>> getOrthographicSize() { return m_orthographicSize; }
        void setOrthographicSize(float val, const Scope<Time>& t){ m_orthographicSize->setVal(std::array<float, 1>{{val}}, t); recalculateProjection(); }
        // void setPerspectiveFOV(float val){ m_perspectiveFOV->setVal(val); recalculateProjection(); }
        Ref<StepController<1>> getOrthographicNearClip() { return m_orthographicNearClip; }
        void setOrthographicNearClip(float val, const Scope<Time>& t){ m_orthographicNearClip->setVal(std::array<float, 1>{{val}}, t); recalculateProjection(); }
        Ref<StepController<1>> getOrthographicFarClip() { return m_orthographicFarClip; }
        void setOrthographicFarClip(float val, const Scope<Time>& t){ m_orthographicFarClip->setVal(std::array<float, 1>{{val}}, t); recalculateProjection(); }


        float getAspectRatio() const { return m_aspectRatio; }
        
        ProjectionType getProjectionType() const { return m_projectionType; }
        void setProjectionType(ProjectionType type) { m_projectionType = type; recalculateProjection(); }

    private:
        void recalculateProjection(const Scope<Time>& t);
        ProjectionType m_projectionType = ProjectionType::Perspective;
        Ref<StepController<1>> m_orthographicSize {CreateRef<StepController<1>>(10.0f)};
        Ref<StepController<1>> m_orthographicNearClip {CreateRef<StepController<1>>(-1.0f)};
        Ref<StepController<1>> m_orthographicFarClip {CreateRef<StepController<1>>(1.0f)};

        Ref<StepController<1>> m_perspectiveFOV{CreateRef<StepController<1>>(glm::radians(45.0f))};
        Ref<StepController<1>> m_perspectiveNearClip {CreateRef<StepController<1>>(0.001f)};
        Ref<StepController<1>> m_perspectiveFarClip {CreateRef<StepController<1>>(1000.0f)};

        float m_aspectRatio{0};
    };
}

