#pragma once

#include <glm/glm.hpp>


namespace ChoreoApp {
    class Camera{
    public:
        Camera() = default;
        Camera(const glm::mat4& projectionMatrix)
            : m_projectionMatrix{projectionMatrix} {}
        virtual ~Camera() = default;

        const glm::mat4& getProjectionMatrix() const { return m_projectionMatrix; }

    protected:
        glm::mat4 m_projectionMatrix;
    };


    // this will get deprecated soon 
    // a 2d camera for specific view (top/bot/right/left)
    class OrthographicCamera{
    public:
        OrthographicCamera(float left, float right, float bottom, float top);
        void setProjection(float left, float right, float bottom, float top);

        const glm::vec3& getPosition() const { return m_position; }
        void setPosition(const glm::vec3& position) { m_position = position; recalculateViewMatrix();}

        float getRotation() const { return m_rotation; }
        void setRotation(float rotation) { m_rotation = rotation; recalculateViewMatrix();}

        const glm::mat4& getProjectionMatrix() const{ return m_projectionMatrix; }
        const glm::mat4& getViewMatrix() const{ return m_projectionMatrix; }
        const glm::mat4& getViewProjectionMatrix() const{ return m_viewProjectionMatrix; }

    private:
        void recalculateViewMatrix();


        glm::mat4 m_projectionMatrix{glm::mat4{1}};
        glm::mat4 m_viewMatrix;
        // for caching 
        glm::mat4 m_viewProjectionMatrix;

        glm::vec3 m_position{0,0,0};
        float m_rotation { 0.0f };

    };

}
