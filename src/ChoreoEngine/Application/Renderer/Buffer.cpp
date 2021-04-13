#include "cepch.h"
#include "Buffer.h"

#include "Application/Renderer/Renderer.h"
#include "Application/Platform/OpenGL/OpenGLBuffer.h"

namespace ChoreoEngine{

    VertexBuffer* VertexBuffer::create(float* vertices, uint32_t size){
        switch ( Renderer::getAPI() ){
            case RendererAPI::API::None: CE_CORE_ASSERT(false, "RendererAPI:None is currently not supported!");
            case RendererAPI::API::OpenGL: return new OpenGLVertexBuffer(vertices, size);
        }

        CE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    IndexBuffer* IndexBuffer::create(uint32_t* indices, uint32_t count){
        switch ( Renderer::getAPI() ){
            case RendererAPI::API::None: CE_CORE_ASSERT(false, "RendererAPI:None is currently not supported!");
            case RendererAPI::API::OpenGL: return new OpenGLIndexBuffer(indices, count);
        }

        CE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}
