#pragma once

#include "Application/Renderer/Buffer.h"


namespace ChoreoEngine{
    
    class OpenGLVertexBuffer : public VertexBuffer{
    public:
        OpenGLVertexBuffer( uint32_t size);
        OpenGLVertexBuffer( float* vertices, uint32_t size);
        virtual ~OpenGLVertexBuffer();
        virtual void bind() const override;
        virtual void unbind() const override;

        virtual const BufferLayout& getLayout() const override { return m_layout; };
        virtual void setLayout(const BufferLayout& layout) override { m_layout = layout; }
        virtual void setData(const void* data, uint32_t size) override;
    
    private:
        uint32_t m_rendererId;
        BufferLayout m_layout;
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



