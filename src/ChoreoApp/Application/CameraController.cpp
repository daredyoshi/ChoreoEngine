#include "capch.h"
#include "Application/CameraController.h"
#include "Application/Input.h"
#include "Application/KeyCodes.h"
#include "Application/Events/Event.h"



namespace ChoreoApp {
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
        :
            m_aspectRatio{aspectRatio}, 
            m_bounds({ -m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel }),
            m_camera(m_bounds.left, m_bounds.right, m_bounds.bottom, m_bounds.top),
            m_allowRotation{rotation}
	{
        CE_PROFILE_FUNCTION();  
	}

	bool OrthographicCameraController::onMouseScrolled(MouseScrollEvent& e)
	{
        CE_PROFILE_FUNCTION();  
        m_zoomLevel -=m_zoomSpeed *  e.getYOffset();
        m_zoomLevel = std::max(0.25f, m_zoomLevel);
        recalculateViewMatrix();
        m_translationSpeed = m_zoomLevel;
		return false;
	}

    void OrthographicCameraController::recalculateViewMatrix(){
        m_bounds = { -m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel };
        m_camera.setProjection(m_bounds.left, m_bounds.right, m_bounds.bottom, m_bounds.top);
    }

	bool OrthographicCameraController::onWindowResized(WindowResizeEvent& e)
	{
        CE_PROFILE_FUNCTION();  
        m_aspectRatio = (float)e.getWidth() / (float)e.getHeight();
        recalculateViewMatrix();
		return false;
	}

	void OrthographicCameraController::onEvent(Event& e){
        CE_PROFILE_FUNCTION();  
        EventDispatcher dispatcher(e);
        dispatcher.dispatch<MouseScrollEvent>(CE_BIND_EVENT_FN(OrthographicCameraController::onMouseScrolled)); 
        dispatcher.dispatch<WindowResizeEvent>(CE_BIND_EVENT_FN(OrthographicCameraController::onWindowResized)) ;
	}

	void OrthographicCameraController::onUpdate(Timestep timestep)
	{
        CE_PROFILE_FUNCTION();  
        if(Input::isKeyPressed(CE_KEY_S)){
            m_position.x -= m_translationSpeed * timestep; 
        }
        if(Input::isKeyPressed(CE_KEY_F)){
            m_position.x += m_translationSpeed* timestep; 
        }
        if(Input::isKeyPressed(CE_KEY_D)){
            m_position.y -= m_translationSpeed* timestep; 
        }
        if(Input::isKeyPressed(CE_KEY_E)){
            m_position.y += m_translationSpeed* timestep; 
        }
        if(m_allowRotation){
            if(ChoreoApp::Input::isKeyPressed(CE_KEY_W))
                m_rotation += m_rotationSpeed * timestep;

            if(ChoreoApp::Input::isKeyPressed(CE_KEY_R))
                m_rotation -= m_rotationSpeed * timestep;
            m_camera.setRotation(m_rotation);
        }
        m_camera.setPosition(m_position);
	}

    void OrthographicCameraController::resize(const float width, const float height){ 
        CE_PROFILE_FUNCTION();  
        m_aspectRatio = width / height;
        recalculateViewMatrix();
    }

}
