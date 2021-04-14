#pragma once

#include "Application/Renderer/Shader.h"
#include <glm/glm.hpp>


namespace ChoreoEngine {
    class OpenGLShader : public Shader{
    public:
        OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
        ~OpenGLShader();

        void bind() const;
        void unbind() const;

        void uploadUniformInt(const std::string& name, const int val);

        void uploadUniformFloat4(const std::string& name, const glm::vec4& val);
        void uploadUniformFloat3(const std::string& name, const glm::vec3& val);
        void uploadUniformFloat2(const std::string& name, const glm::vec2& val);
        void uploadUniformFloat(const std::string& name, const float val);
        void uploadUniformMat4(const std::string& name, const glm::mat4& val);
        void uploadUniformMat3(const std::string& name, const glm::mat3& val);

    private:
        uint32_t m_rendererId;
        unsigned int m_vertexShader;
    };
}

