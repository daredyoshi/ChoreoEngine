#pragma once

#include "Application/Renderer/VertexArray.h"
#include <memory>

namespace ChoreoApp{
    class OpenGLVertexArray : public VertexArray{
    public:
        OpenGLVertexArray();
        virtual ~OpenGLVertexArray();

        virtual void bind() const override;
        virtual void unbind() const override;

        virtual void addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
        virtual void setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

        virtual const std::vector<Ref<VertexBuffer>>& getVertexBuffers() const override { return m_vertexBuffers; };
        virtual const Ref<IndexBuffer>& getIndexBuffer() const override { return m_indexBuffer; };

    private:
        std::vector<Ref<VertexBuffer>> m_vertexBuffers;
        Ref<IndexBuffer> m_indexBuffer;
        uint32_t m_rendererId;
    };
}
