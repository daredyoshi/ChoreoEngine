#pragma once 
#include "Application/Renderer/Framebuffer.h"


namespace ChoreoApp {
    class OpenGLFramebuffer : public Framebuffer{
    public:
        OpenGLFramebuffer(FramebufferSpecification& spec);
        virtual ~OpenGLFramebuffer();

        // recreate the framebuffer by invalidating it
        virtual void invalidate() override;


        // we will probably be removing these in the future
        virtual void bind() const override;
        virtual void unbind() const override;
        virtual void resize(uint32_t width, uint32_t height) override;

        
        virtual uint32_t getColorAttachmenRendererID() const override { return m_colorAttachement; };
        virtual const FramebufferSpecification& getSpecification() const override { return m_spec; }
        virtual const FramebufferSpecification& getSpecification() override { return m_spec; }

    private:
        uint32_t m_rendererID{0};
        // these attachments will be customizable in the future
        uint32_t m_colorAttachement{0};
        uint32_t m_depthAttachement{0};
        FramebufferSpecification& m_spec; 
    };
}
