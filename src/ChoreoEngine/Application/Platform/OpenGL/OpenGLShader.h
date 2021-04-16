#pragma once

#include "Application/Renderer/Shader.h"
#include <glm/glm.hpp>
#include <unordered_map>

//TODO: REMOVE
typedef unsigned int GLenum;

namespace ChoreoEngine {
    class OpenGLShader : public Shader{
    public:
        OpenGLShader(const std::string& path);
        OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
        ~OpenGLShader();

        void bind() const override;
        void unbind() const override;

        const std::string& getName() const override { return m_name; };

        void uploadUniformInt(const std::string& name, const int val);

        void uploadUniformFloat4(const std::string& name, const glm::vec4& val);
        void uploadUniformFloat3(const std::string& name, const glm::vec3& val);
        void uploadUniformFloat2(const std::string& name, const glm::vec2& val);
        void uploadUniformFloat(const std::string& name, const float val);
        void uploadUniformMat4(const std::string& name, const glm::mat4& val);
        void uploadUniformMat3(const std::string& name, const glm::mat3& val);

    private:
        std::string readFile(const std::string& path);
        // this will split up a single shader file into all the shaders within
        std::unordered_map<GLenum, std::string> preProcess(const std::string& shaderSrc);
        void compile(const std::unordered_map<GLenum, std::string>& shaderSources);
        uint32_t m_rendererId;
        unsigned int m_vertexShader;
        std::string m_name;
    };
}

