#pragma once


#include "Application/Renderer/Camera.h"

namespace ChoreoApp {
    class SceneCamera : public Camera{
    public:
        SceneCamera();
        virtual ~SceneCamera() override {};
        
        void setOrthographic(float size, float nearClip, float farClip);
        void setViewportSize(const uint32_t width,const uint32_t height);

        float getOrthographicSize() const { return m_orthographicSize; }
        void setOrthographicSize(float orthographicSize){ m_orthographicSize = orthographicSize; recalculateProjection();}

    private:
        void recalculateProjection();
        float m_orthographicSize = 10.0f;
        float m_orthographicNear = -1.0f;
        float m_orthographicFar = 1.0f;

        float m_aspectRatio{0};
    };
}

