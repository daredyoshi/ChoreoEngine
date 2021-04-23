#include "capch.h"
#include "Camera.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/matrix.hpp"

namespace ChoreoApp{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
        : 
            m_projectionMatrix{glm::ortho(left, right, bottom, top, -1.0f, 1.0f)},
            m_viewMatrix{1}
    {
        CE_PROFILE_FUNCTION();  
        m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
	}

    void OrthographicCamera::setProjection(float left, float right, float bottom, float top){
        CE_PROFILE_FUNCTION();  
        m_projectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
        m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
    }

	void OrthographicCamera::recalculateViewMatrix(){
        CE_PROFILE_FUNCTION();  
        glm::mat4 xform =glm::rotate(glm::mat4(1.09f), glm::radians(m_rotation), glm::vec3(0, 0, 1)) * glm::translate(glm::mat4(1.0f), m_position) ;
        
        m_viewMatrix = glm::inverse(xform);
        m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;

	}

}
