#include "cepch.h"
#include "Application/CameraController.h"
#include "Application/Input.h"
#include "Application/KeyCodes.h"
#include "Application/Events/Event.h"



namespace ChoreoEngine {
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
        : m_aspectRatio{aspectRatio}, m_camera(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel),
            m_allowRotation{rotation}
	{
        CE_PROFILE_FUNCTION();  
	}

	bool OrthographicCameraController::onMouseScrolled(MouseScrollEvent& e)
	{
        CE_PROFILE_FUNCTION();  
        m_zoomLevel -=m_zoomSpeed *  e.getYOffset();
        m_zoomLevel = std::max(0.25f, m_zoomLevel);
        m_camera.setProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
        m_translationSpeed = m_zoomLevel;
		return false;
	}

	bool OrthographicCameraController::onWindowResized(WindowResizeEvent& e)
	{
        CE_PROFILE_FUNCTION();  
        m_aspectRatio = (float)e.getWidth() / (float)e.getHeight();
        m_camera.setProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
		return false;
	}

	void OrthographicCameraController::onEvent(Event& e){
        CE_PROFILE_FUNCTION();  
        EventDispatcher dispatcher(e);
        dispatcher.dispatch<MouseScrollEvent>(CE_BIND_EVENT_FN(OrthographicCameraController::onMouseScrolled)); 
        dispatcher.dispatch<WindowResizeEvent>(CE_BIND_EVENT_FN(OrthographicCameraController::onWindowResized)) ;
	}

	void OrthographicCameraController::onUpdate(TimeStep timestep)
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
            if(ChoreoEngine::Input::isKeyPressed(CE_KEY_W))
                m_rotation += m_rotationSpeed * timestep;

            if(ChoreoEngine::Input::isKeyPressed(CE_KEY_R))
                m_rotation -= m_rotationSpeed * timestep;
            m_camera.setRotation(m_rotation);
        }
        m_camera.setPosition(m_position);
	}

}
