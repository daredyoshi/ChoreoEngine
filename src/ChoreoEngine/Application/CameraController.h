#pragma once

#include "Application/Renderer/Camera.h"
#include "Application/Timestep.h"

#include "Application/Events/Application.h"
#include "Application/Events/MouseEvent.h"

namespace ChoreoEngine{

    // this will basically be a viewport into our view
    class OrthographicCameraController {
    public:
        OrthographicCameraController(float aspectRatio, bool rotation = false);  
        
        void onUpdate(TimeStep timestep);
        void onEvent(Event& e);

        OrthographicCamera& getCamera() { return m_camera; }
        const OrthographicCamera& getCamera() const { return m_camera; }

    private:
        bool onMouseScrolled(MouseScrollEvent& e);
        bool onWindowResized(WindowResizeEvent& e);

        float m_aspectRatio;
        float m_zoomLevel{1.0f};

        OrthographicCamera m_camera;
        
        bool m_allowRotation = false;
        float m_rotation{0.0f};
        glm::vec3 m_position{0};

        float m_translationSpeed{1.0};
        float m_rotationSpeed{50.0};
        float m_zoomSpeed{0.2};
    };
}
