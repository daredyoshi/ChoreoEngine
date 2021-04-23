#pragma once

#include "Application/Renderer/Camera.h"
#include "Application/Timestep.h"

#include "Application/Events/Application.h"
#include "Application/Events/MouseEvent.h"

namespace ChoreoApp{
    struct OrthographicCameraBounds{
        float left, right;
        float bottom, top;

        float getWidth() const { return right - left; }
        float getHeight() const { return top - bottom; }
    };

    // this will basically be a viewport into our view
    class OrthographicCameraController {
    public:
        OrthographicCameraController(float aspectRatio, bool rotation = false);  
        
        void onUpdate(TimeStep timestep);
        void onEvent(Event& e);
        const OrthographicCameraBounds& getBounds() const { return m_bounds; }

        OrthographicCamera& getCamera() { return m_camera; }
        const OrthographicCamera& getCamera() const { return m_camera; }

        float getZoomLevel() const { return m_zoomLevel; }
        void setZoomLevel(float level) { m_zoomLevel = level; recalculateViewMatrix(); }

        void recalculateViewMatrix();

    private:
        bool onMouseScrolled(MouseScrollEvent& e);
        bool onWindowResized(WindowResizeEvent& e);

        float m_aspectRatio;
        float m_zoomLevel{1.0f};
        float m_zoomSpeed{0.25f};

        OrthographicCameraBounds m_bounds;
        OrthographicCamera m_camera;
        
        bool m_allowRotation = false;
        float m_rotation{0.0f};
        glm::vec3 m_position{0};

        float m_translationSpeed{1.0};
        float m_rotationSpeed{50.0};

    };
}
