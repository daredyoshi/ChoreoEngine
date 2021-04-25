#pragma once 
#include "Application/Renderer/Framebuffer.h"


namespace ChoreoApp {
    class OpenGLFramebuffer : public Framebuffer{
    public:
        OpenGLFramebuffer(const FramebufferSpecification& spec);

        // recreate the framebuffer by invalidating it
        virtual void invalidate() override;


        // we will probably be removing these in the future
        virtual void bind() const override;
        virtual void unbind() const override;

        
        virtual uint32_t getColorAttachmenRendererID() const override { return m_colorAttachement; };
        virtual const FramebufferSpecification& getSpecification() const override { return m_spec; }
        virtual const FramebufferSpecification& getSpecification() override { return m_spec; }

        virtual ~OpenGLFramebuffer();
    private:
        uint32_t m_rendererID;
        // these attachments will be customizable in the future
        uint32_t m_colorAttachement;
        uint32_t m_depthAttachement;
        const FramebufferSpecification& m_spec; 
    };
}
