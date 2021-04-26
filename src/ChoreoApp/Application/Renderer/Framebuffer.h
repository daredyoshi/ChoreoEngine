#pragma once

#include "capch.h"
#include "Application/Core.h"

namespace ChoreoApp {

    struct FramebufferSpecification{
        uint32_t width, height; 
        uint32_t samples{1};

        // this frame buffer is rendering to swap chain
        // false renders to the screen
        bool swapChainTarget{false};

    };

    class Framebuffer {
    public:
        virtual ~Framebuffer() = default;
        virtual const FramebufferSpecification& getSpecification() = 0;
        virtual const FramebufferSpecification& getSpecification() const = 0;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;
        virtual void resize(uint32_t width, uint32_t height)= 0;

        virtual void invalidate() = 0;

        // if rendering to a texture in ImGui this is what you give it
        virtual uint32_t getColorAttachmenRendererID() const = 0;

        static Ref<Framebuffer> create(FramebufferSpecification& spec);
    };
}
