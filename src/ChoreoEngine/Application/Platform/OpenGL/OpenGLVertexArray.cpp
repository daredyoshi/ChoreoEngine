#include "OpenGLVertexArray.h"
#include <glad/glad.h>
#include "Application/Renderer/Buffer.h"
#include "cepch.h"

static GLenum ShaderDataTypeToOpenGLBaseType(ChoreoEngine::ShaderDataType type){
    switch(type){
        case ChoreoEngine::ShaderDataType::Float:
        case ChoreoEngine::ShaderDataType::Float2:
        case ChoreoEngine::ShaderDataType::Float3:
        case ChoreoEngine::ShaderDataType::Float4:
        case ChoreoEngine::ShaderDataType::Mat3:
        case ChoreoEngine::ShaderDataType::Mat4:
            return GL_FLOAT;
        case ChoreoEngine::ShaderDataType::Int:
        case ChoreoEngine::ShaderDataType::Int2:
        case ChoreoEngine::ShaderDataType::Int3:
        case ChoreoEngine::ShaderDataType::Int4:
            return GL_INT;
        case ChoreoEngine::ShaderDataType::Bool:
            return GL_BOOL;
        default:
            CE_CORE_ASSERT(false, "Unknown ShaderDataType!");
            return 0;
    }
}

ChoreoEngine::OpenGLVertexArray::OpenGLVertexArray() 
{
    CE_PROFILE_FUNCTION();  
    glCreateVertexArrays(1, &m_rendererId);
}

ChoreoEngine::OpenGLVertexArray::~OpenGLVertexArray() 
{
    CE_PROFILE_FUNCTION();  
    glDeleteVertexArrays(1, &m_rendererId);
}

void ChoreoEngine::OpenGLVertexArray::addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) 
{
    CE_PROFILE_FUNCTION();  
    CE_CORE_ASSERT(vertexBuffer->getLayout().getElements().size(), "Vertex Buffer layout has not been set!");

    glBindVertexArray(m_rendererId);
    vertexBuffer->bind();

    uint32_t idx = 0;
    const auto& layout = vertexBuffer->getLayout();
    for(const auto& element : layout){
        glEnableVertexAttribArray(idx);
        glVertexAttribPointer(idx, 
                element.getComponentCount(), 
                ShaderDataTypeToOpenGLBaseType(element.type), 
                element.normalized ? GL_TRUE : GL_FALSE, 
                layout.getStride(), 
                (const void*)(uintptr_t)element.offset);
        idx++;
    }

    m_vertexBuffers.push_back(vertexBuffer);
}

void ChoreoEngine::OpenGLVertexArray::bind() const 
{
    CE_PROFILE_FUNCTION();  
    glBindVertexArray(m_rendererId);
}

void ChoreoEngine::OpenGLVertexArray::setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) 
{
    CE_PROFILE_FUNCTION();  
    glBindVertexArray(m_rendererId);
    indexBuffer->bind();

    m_indexBuffer = indexBuffer;
}

void ChoreoEngine::OpenGLVertexArray::unbind() const 
{
    CE_PROFILE_FUNCTION();  
    glBindVertexArray(0);
}

