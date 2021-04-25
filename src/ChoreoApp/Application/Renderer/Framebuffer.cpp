
#include "capch.h"
#include "Framebuffer.h"
#include "Renderer.h"
#include "Application/Platform/OpenGL/OpenGLFramebuffer.h"


namespace ChoreoApp {
    Ref<Framebuffer> Framebuffer::create(FramebufferSpecification& spec){
        switch ( Renderer::getAPI() ){
            case RendererAPI::API::None: CE_CORE_ASSERT(false, "RendererAPI:None is currently not supported!");
            case RendererAPI::API::OpenGL: return CreateRef<OpenGLFramebuffer>(spec);
        }

        CE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}



