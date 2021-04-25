#include "OpenGLVertexArray.h"
#include <glad/glad.h>
#include "Application/Renderer/Buffer.h"
#include "capch.h"

static GLenum ShaderDataTypeToOpenGLBaseType(ChoreoApp::ShaderDataType type){
    switch(type){
        case ChoreoApp::ShaderDataType::Float:
        case ChoreoApp::ShaderDataType::Float2:
        case ChoreoApp::ShaderDataType::Float3:
        case ChoreoApp::ShaderDataType::Float4:
        case ChoreoApp::ShaderDataType::Mat3:
        case ChoreoApp::ShaderDataType::Mat4:
            return GL_FLOAT;
        case ChoreoApp::ShaderDataType::Int:
        case ChoreoApp::ShaderDataType::Int2:
        case ChoreoApp::ShaderDataType::Int3:
        case ChoreoApp::ShaderDataType::Int4:
            return GL_INT;
        case ChoreoApp::ShaderDataType::Bool:
            return GL_BOOL;
        default:
            CE_CORE_ASSERT(false, "Unknown ShaderDataType!");
            return 0;
    }
}

ChoreoApp::OpenGLVertexArray::OpenGLVertexArray() 
{
    CE_PROFILE_FUNCTION();  
    glCreateVertexArrays(1, &m_rendererId);
}

ChoreoApp::OpenGLVertexArray::~OpenGLVertexArray() 
{
    CE_PROFILE_FUNCTION();  
    glDeleteVertexArrays(1, &m_rendererId);
}

void ChoreoApp::OpenGLVertexArray::addVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) 
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

void ChoreoApp::OpenGLVertexArray::bind() const 
{
    CE_PROFILE_FUNCTION();  
    glBindVertexArray(m_rendererId);
}

void ChoreoApp::OpenGLVertexArray::setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) 
{
    CE_PROFILE_FUNCTION();  
    glBindVertexArray(m_rendererId);
    indexBuffer->bind();

    m_indexBuffer = indexBuffer;
}

void ChoreoApp::OpenGLVertexArray::unbind() const 
{
    CE_PROFILE_FUNCTION();  
    glBindVertexArray(0);
}

