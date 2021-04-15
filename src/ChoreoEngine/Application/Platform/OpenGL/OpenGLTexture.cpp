#include "cepch.h"
#include "OpenGLTexture.h"
#include <glad/glad.h>
#include "stb_image.h"

ChoreoEngine::OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
    : m_path(path) { 
    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    CE_CORE_ASSERT(data, "Failed to load image");

    m_width = width;
    m_height = height;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererId);
    glTextureStorage2D(m_rendererId, 1, GL_RGB8, m_width, m_height);

    glTextureParameteri(m_rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_rendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureSubImage2D(m_rendererId, 0, 0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, data);
    
    stbi_image_free(data);
}

ChoreoEngine::OpenGLTexture2D::~OpenGLTexture2D(){
    glDeleteTextures(1, &m_rendererId);
}

void ChoreoEngine::OpenGLTexture2D::bind(uint32_t slot) const  
{
    glBindTextureUnit(slot, m_rendererId);
}

