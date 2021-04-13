#include "VertexArray.h"
#include "cepch.h"
#include "Renderer.h"
#include "Application/Platform/OpenGL/OpenGLVertexArray.h"


namespace ChoreoEngine {

    VertexArray* VertexArray::create(){
        switch ( Renderer::getAPI() ){
            case RendererAPI::API::None: CE_CORE_ASSERT(false, "RendererAPI:None is currently not supported!");
            case RendererAPI::API::OpenGL: return new OpenGLVertexArray();
        }

        CE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}
