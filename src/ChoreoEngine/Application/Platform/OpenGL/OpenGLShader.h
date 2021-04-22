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

        virtual void setMat4(const std::string& name, const glm::mat4& val) const override ;
        virtual void setInt(const std::string& name, const int val) const override ;
        virtual void setIntArray(const std::string& name, const int* values, uint32_t count) const override ;
        virtual void setFloat(const std::string& name, const float val)const override;
        virtual void setFloat3(const std::string& name, const glm::vec3& val)const override;
        virtual void setFloat4(const std::string& name, const glm::vec4& val)const override;

        void uploadUniformInt(const std::string& name, const int val) const;
        void uploadUniformIntArray(const std::string& name, const int* values, const uint32_t count) const;
        void uploadUniformFloat4(const std::string& name, const glm::vec4& val) const;
        void uploadUniformFloat3(const std::string& name, const glm::vec3& val) const;
        void uploadUniformFloat2(const std::string& name, const glm::vec2& val) const;
        void uploadUniformFloat(const std::string& name, const float val) const;
        void uploadUniformMat4(const std::string& name, const glm::mat4& val) const;
        void uploadUniformMat3(const std::string& name, const glm::mat3& val) const;

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

