#include "cepch.h"
#include "Application/Platform/OpenGL/OpenGLBuffer.h"

#include <glad/glad.h>

////////////////////////////////////////////////////////////
// Vertex Buffer ////////////////////////////////////////////
////////////////////////////////////////////////////////////

ChoreoEngine::OpenGLVertexBuffer::OpenGLVertexBuffer( float* vertices, uint32_t count)
{
    glCreateBuffers(1, &m_rendererId);
    this->bind();
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * count, vertices, GL_STATIC_DRAW);    
}

ChoreoEngine::OpenGLVertexBuffer::~OpenGLVertexBuffer(){
    glDeleteBuffers(1, &m_rendererId);
}

void ChoreoEngine::OpenGLVertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
}

void ChoreoEngine::OpenGLVertexBuffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

////////////////////////////////////////////////////////////
// Index Buffer ////////////////////////////////////////////
////////////////////////////////////////////////////////////

ChoreoEngine::OpenGLIndexBuffer::OpenGLIndexBuffer( uint32_t* indices, uint32_t count )
    : m_count{count}
{
    glCreateBuffers(1, &m_rendererId);
    this->bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * count, indices, GL_STATIC_DRAW);
}

ChoreoEngine::OpenGLIndexBuffer::~OpenGLIndexBuffer(){
    glDeleteBuffers(1, &m_rendererId);
}

void ChoreoEngine::OpenGLIndexBuffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId);
}

void ChoreoEngine::OpenGLIndexBuffer::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

