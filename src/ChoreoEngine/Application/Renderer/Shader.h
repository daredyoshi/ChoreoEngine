#pragma once

#include "cepch.h"

namespace ChoreoEngine {
    class Shader{
    public:
        Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
        ~Shader();

        void Bind() const;
        void Unbind() const;

    private:
        uint32_t m_rendererId;
        unsigned int m_vertexShader;
    };
}
