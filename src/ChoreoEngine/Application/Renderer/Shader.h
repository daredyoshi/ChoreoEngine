#pragma once
#include "cepch.h"

namespace ChoreoEngine {
    class Shader{
    public:
        ~Shader() {};

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        static Ref<Shader> create(const std::string& path);
        static Ref<Shader> create(const std::string& vertexSrc, const std::string& fragmentSrc);


    private:
        uint32_t m_rendererId;
        unsigned int m_vertexShader;
    };
}
