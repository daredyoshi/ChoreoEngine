#pragma once

#include "RenderCommand.h"
#include "RendererAPI.h"
#include "Camera.h"
#include "Shader.h"
#include <memory>

namespace ChoreoEngine {
    class Renderer {
    public:
        static void init();
        static void onWindowResize(uint32_t width, uint32_t height);
        static void beginScene(OrthographicCamera& cam);
        static void endScene();

	    static void submit(const Ref<Shader>& shader, 
                const Ref<VertexArray>& vertexArray, 
                const glm::mat4& xform=glm::mat4{1});

        inline static RendererAPI::API getAPI() { return RendererAPI::getAPI(); }
        struct SceneData{
            glm::mat4 viewPrjectionMatrix;
        };
    private:

        static SceneData* m_sceneData;
        
    };
}
