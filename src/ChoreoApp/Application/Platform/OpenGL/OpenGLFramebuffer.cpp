
#include "capch.h"
#include "OpenGLFramebuffer.h"
#include <glad/glad.h>

namespace ChoreoApp{
    OpenGLFramebuffer::OpenGLFramebuffer(FramebufferSpecification& spec)
        : m_spec{spec} {
                
    }

    OpenGLFramebuffer::~OpenGLFramebuffer(){
        glDeleteFramebuffers(1, &m_rendererID);
        glDeleteTextures(1, &m_colorAttachement);
        glDeleteTextures(1, &m_depthAttachement);
    }

    void OpenGLFramebuffer::invalidate(){
        // if there already is a renderer id delete the old and create new
        if(m_rendererID == 0){
            glDeleteFramebuffers(1, &m_rendererID);
            glDeleteTextures(1, &m_colorAttachement);
            glDeleteTextures(1, &m_depthAttachement);
        }

        glCreateFramebuffers(1, &m_rendererID); 
        glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_colorAttachement);
        glBindTexture(GL_TEXTURE_2D, m_colorAttachement);
        // this is different than texture storage because you can sample from it
        // we could use our texture class for this, but it's simpler to keep it here imho
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_spec.width, m_spec.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glad_glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorAttachement, 0);

        // also create a depth buffer
        glCreateTextures(GL_TEXTURE_2D, 1, &m_depthAttachement);
        glBindTexture(GL_TEXTURE_2D, m_depthAttachement);
        glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_spec.width, m_spec.height) ;
        // glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_spec.width, m_spec.height, 0, 
        //         GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthAttachement, 0);

        CE_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!"); 

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    }

    void OpenGLFramebuffer::bind() const{
        glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);
        glViewport(0, 0, m_spec.width, m_spec.height);
    }

    void OpenGLFramebuffer::unbind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFramebuffer::resize(uint32_t width, uint32_t height){
        m_spec.width = width;
        m_spec.height = height;
        invalidate();
    }
}
