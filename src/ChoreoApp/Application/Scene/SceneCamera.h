#pragma once


#include "Application/Renderer/Camera.h"
#include "Controller.h"
#include "glm/glm.hpp"
#include <memory>

namespace ChoreoApp {
    class SceneCamera : public Camera{
    public:
        enum class ProjectionType { Perspective = 0, Orthographic = 1 };


        SceneCamera();
        virtual ~SceneCamera() override {};

        void setViewportSize(const uint32_t width,const uint32_t height);

        // PERSPECTIVE 
        Ref<FloatController> getPerspectiveFOV() { return m_perspectiveFOV; }
        void setPerspectiveFOV(float val, const Scope<Time>& t){ m_perspectiveFOV->getKey(t)->setVal(val); recalculateProjection(t); }
        Ref<FloatController> getPerspectiveNearClip() { return m_perspectiveNearClip; }
        void setPerspectiveNearClip(float val, const Scope<Time>& t){ m_perspectiveNearClip->getKey(t)->setVal(val); recalculateProjection(t); }
        Ref<FloatController> getPerspectiveFarClip() { return m_perspectiveFarClip; }
        void setPerspectiveFarClip(float val, const Scope<Time>& t){ m_perspectiveFarClip->getKey(t)->setVal(val); recalculateProjection(t); }
        
        // ORTHOGRAPHIC
        Ref<FloatController> getOrthographicSize() { return m_orthographicSize; }
        void setOrthographicSize(float val, const Scope<Time>& t){ m_orthographicSize->getKey(t)->setVal(val); recalculateProjection(t); }
        // void setPerspectiveFOV(float val){ m_perspectiveFOV->setVal(val); recalculateProjection(); }
        Ref<FloatController> getOrthographicNearClip() { return m_orthographicNearClip; }
        void setOrthographicNearClip(float val, const Scope<Time>& t){ m_orthographicNearClip->getKey(t)->setVal(val); recalculateProjection(t); }
        Ref<FloatController> getOrthographicFarClip() { return m_orthographicFarClip; }
        void setOrthographicFarClip(float val, const Scope<Time>& t){ m_orthographicFarClip->getKey(t)->setVal(val); recalculateProjection(t); }


        float getAspectRatio() const { return m_aspectRatio; }
        
        ProjectionType getProjectionType() const { return m_projectionType; }
        void setProjectionType(ProjectionType type) { m_projectionType = type; recalculateProjection(); }

    private:
        void recalculateProjection(const Scope<Time>& t);
        void recalculateProjection();
        ProjectionType m_projectionType = ProjectionType::Perspective;
        Ref<FloatController> m_orthographicSize {std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>(10.0f))};
        Ref<FloatController> m_orthographicNearClip {std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>(-1.0f))};
        Ref<FloatController> m_orthographicFarClip {std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>(1.0f))};

        Ref<FloatController> m_perspectiveFOV{std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>(glm::radians(45.0f)))};
        Ref<FloatController> m_perspectiveNearClip {std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>(0.001f))};
        Ref<FloatController> m_perspectiveFarClip {std::static_pointer_cast<FloatController>(CreateRef<StaticFloatController>(1000.0f))};
        // std::shared_ptr<FloatController> m_perspectiveFarClip {std::static_pointer_cast<FloatController>(std::make_shared<FloatStepController>(1000.0f))};

        float m_aspectRatio{0};
        Time t_cache{Time{0}};
    };
}

