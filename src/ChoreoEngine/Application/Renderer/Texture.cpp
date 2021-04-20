#include "cepch.h"
#include "Texture.h"
#include "Renderer.h"
#include "Application/Platform/OpenGL/OpenGLTexture.h"

namespace ChoreoEngine {
    Ref<Texture2D> Texture2D::create(const std::string& path){
        switch ( Renderer::getAPI() ){
            case RendererAPI::API::None: CE_CORE_ASSERT(false, "RendererAPI:None is currently not supported!");
            case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(path);
        }

        CE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Ref<Texture2D> Texture2D::create(uint32_t width, uint32_t height){
        switch ( Renderer::getAPI() ){
            case RendererAPI::API::None: CE_CORE_ASSERT(false, "RendererAPI:None is currently not supported!");
            case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(width, height);
        }

        CE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}
