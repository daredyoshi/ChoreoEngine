#pragma once 

#include "RendererAPI.h"
#include <memory>

namespace ChoreoEngine{
    class RenderCommand{
    public:
        inline static void init(){
            s_rendererAPI->init();
        }
        inline static void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height){
            s_rendererAPI->setViewport(x, y, width, height);
        }
        inline static void setClearColor(const glm::vec4& color){
            s_rendererAPI->setClearColor(color);
        }
        inline static void clear(){
            s_rendererAPI->clear();
        }
        inline static void DrawIndexed( const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0){
            s_rendererAPI->drawIndexed(vertexArray, indexCount);
        }

    private:
        static RendererAPI* s_rendererAPI;
    };


}
