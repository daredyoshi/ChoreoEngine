#include "cepch.h"
#include "Buffer.h"

#include "Application/Renderer/Renderer.h"
#include "Application/Platform/OpenGL/OpenGLBuffer.h"

namespace ChoreoEngine{

    VertexBuffer* VertexBuffer::create(float* vertices, uint32_t size){
        switch ( Renderer::getAPI() ){
            case RendererAPI::None: CE_CORE_ASSERT(false, "RendererAPI:None is currently not supported!");
            case RendererAPI::OpenGL: return new OpenGLVertexBuffer(vertices, size);
        }

        CE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    IndexBuffer* IndexBuffer::create(uint32_t* indices, uint32_t size){
        switch ( Renderer::getAPI() ){
            case RendererAPI::None: CE_CORE_ASSERT(false, "RendererAPI:None is currently not supported!");
            case RendererAPI::OpenGL: return new OpenGLIndexBuffer(indices, size);
        }

        CE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}
