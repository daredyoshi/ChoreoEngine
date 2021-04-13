#pragma once

#include "Application/Renderer/Buffer.h"


namespace ChoreoEngine{
    
    class OpenGLVertexBuffer : public VertexBuffer{
    public:
        OpenGLVertexBuffer( float* vertices, uint32_t count );
        virtual ~OpenGLVertexBuffer();
        virtual void bind() const override;
        virtual void unbind() const override;
    
    private:
        uint32_t m_rendererId;
    };

    class OpenGLIndexBuffer : public IndexBuffer{
    public:
        OpenGLIndexBuffer( unsigned int* indices, uint32_t count);
        virtual ~OpenGLIndexBuffer();
        virtual void bind() const override;
        virtual void unbind() const override;
        virtual uint32_t getCount() const override { return m_count; }
    
    private:
        uint32_t m_count;
        uint32_t m_rendererId;
    };
}



