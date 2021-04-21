#include "cepch.h"
#include "Application/Platform/OpenGL/OpenGLBuffer.h"

#include <glad/glad.h>

////////////////////////////////////////////////////////////
// Vertex Buffer ////////////////////////////////////////////
////////////////////////////////////////////////////////////

ChoreoEngine::OpenGLVertexBuffer::OpenGLVertexBuffer( uint32_t size)
{
    CE_PROFILE_FUNCTION();  
    glCreateBuffers(1, &m_rendererId);
    this->bind();
    // assume that if constructing the buffer like this you are 
    // targeting batch rendering which would update the buffer on
    // every frame
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);    
}

ChoreoEngine::OpenGLVertexBuffer::OpenGLVertexBuffer( float* vertices, uint32_t size)
{
    CE_PROFILE_FUNCTION();  
    glCreateBuffers(1, &m_rendererId);
    this->bind();
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);    
}

ChoreoEngine::OpenGLVertexBuffer::~OpenGLVertexBuffer(){
    CE_PROFILE_FUNCTION();  
    glDeleteBuffers(1, &m_rendererId);
}

void ChoreoEngine::OpenGLVertexBuffer::bind() const
{
    CE_PROFILE_FUNCTION();  
    glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
}

void ChoreoEngine::OpenGLVertexBuffer::unbind() const
{
    CE_PROFILE_FUNCTION();  
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void ChoreoEngine::OpenGLVertexBuffer::setData(const void* data, uint32_t size) {
    glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
    // could add an offset at some point but don't need yet
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}



////////////////////////////////////////////////////////////
// Index Buffer ////////////////////////////////////////////
////////////////////////////////////////////////////////////

ChoreoEngine::OpenGLIndexBuffer::OpenGLIndexBuffer( uint32_t* indices, uint32_t count )
    : m_count{count}
{
    CE_PROFILE_FUNCTION();  
    glCreateBuffers(1, &m_rendererId);
    this->bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * count, indices, GL_STATIC_DRAW);
}

ChoreoEngine::OpenGLIndexBuffer::~OpenGLIndexBuffer(){
    CE_PROFILE_FUNCTION();  
    glDeleteBuffers(1, &m_rendererId);
}

void ChoreoEngine::OpenGLIndexBuffer::bind() const
{
    CE_PROFILE_FUNCTION();  
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId);
}

void ChoreoEngine::OpenGLIndexBuffer::unbind() const
{
    CE_PROFILE_FUNCTION();  
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

