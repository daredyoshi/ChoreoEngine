#include "cepch.h"
#include "OpenGLTexture.h"
#include <glad/glad.h>
#include "stb_image.h"


ChoreoEngine::OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
    :  m_width{width}, m_height{height}{

    CE_PROFILE_FUNCTION();  
    m_internalFormat = GL_RGBA8 ;
    m_dataFormat = GL_RGBA;
        

    CE_CORE_ASSERT(m_internalFormat & m_dataFormat, "Image format not supported!")

    glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererId);
    glTextureStorage2D(m_rendererId, 1,m_internalFormat, m_width, m_height);

    glTextureParameteri(m_rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_rendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}


void ChoreoEngine::OpenGLTexture2D::setdata(void* data, uint32_t size) {
    CE_PROFILE_FUNCTION();  
    // make sure the images is the same size
    uint32_t bpc = m_dataFormat == GL_RGBA ? 4 : 3; // bytes per chanel
    CE_CORE_ASSERT(size == m_width * m_height * bpc, "Size does not match Texture size!");
    (void)size;
    glTextureSubImage2D(m_rendererId, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, data);
}


ChoreoEngine::OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
    : m_path(path) { 
    CE_PROFILE_FUNCTION();  
    int width, height, channels;
    stbi_uc* data = nullptr;
    stbi_set_flip_vertically_on_load(1);
    {
        CE_PROFILE_SCOPE(" stbi_load = OpenGLTexture2D::OpenGLTexture2D(const std::string& path)");
        data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    }
    CE_CORE_ASSERT(data, "Failed to load image");


    m_width = width;
    m_height = height;

    GLenum internalFormat{0}; 
    GLenum dataFormat {0};

    if(channels == 3){
        internalFormat = GL_RGB8 ;
        dataFormat = GL_RGB;
    }
    else if (channels == 4){
        internalFormat = GL_RGBA8 ;
        dataFormat = GL_RGBA;
        
    }

    m_internalFormat = internalFormat;
    m_dataFormat = dataFormat;

    CE_CORE_ASSERT(internalFormat & dataFormat, "Image format not supported!")

    glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererId);
    glTextureStorage2D(m_rendererId, 1,internalFormat, m_width, m_height);

    glTextureParameteri(m_rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_rendererId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureSubImage2D(m_rendererId, 0, 0, 0, m_width, m_height, dataFormat, GL_UNSIGNED_BYTE, data);
    
    stbi_image_free(data);
}

ChoreoEngine::OpenGLTexture2D::~OpenGLTexture2D(){
    CE_PROFILE_FUNCTION();  
    glDeleteTextures(1, &m_rendererId);
}

void ChoreoEngine::OpenGLTexture2D::bind(uint32_t slot) const  
{
    CE_PROFILE_FUNCTION();  
    glBindTextureUnit(slot, m_rendererId);
}

