#pragma once
#include "capch.h"
#include <unordered_map>
#include <glm/glm.hpp>

namespace ChoreoApp {
    class Shader{
    public:
        ~Shader() {};

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        static Ref<Shader> create(const std::string& path);
        static Ref<Shader> create(const std::string&  name, const std::string& vertexSrc, const std::string& fragmentSrc);

        virtual const std::string& getName() const = 0;

        virtual void setMat4(const std::string& name, const glm::mat4& val) const = 0;
        virtual void setInt(const std::string& name, const int val) const = 0;
        virtual void setIntArray(const std::string& name, const int* values, uint32_t count) const = 0;
        virtual void setFloat(const std::string& name, const float val)const = 0;
        virtual void setFloat3(const std::string& name, const glm::vec3& val)const = 0;
        virtual void setFloat4(const std::string& name, const glm::vec4& val)const = 0;

    private:
        
        uint32_t m_rendererId;
        unsigned int m_vertexShader;
    };

    class ShaderLibrary{
    public:
        // the actual smart pointer will be copied when it's added to the unordered map
        void add(const Ref<Shader>&);
        void add(const std::string& name, const Ref<Shader>&);
        // load based on the name of the filepath
        Ref<Shader> load(const std::string& path);
        // in case you want a custom name with the same filepath
        Ref<Shader> load(const std::string& name, const std::string& filepath);

        Ref<Shader> get(const std::string& name);

        bool exists(const std::string& name) const;

    private:
        std::unordered_map<std::string, Ref<Shader>> m_shaders;
    };
}
