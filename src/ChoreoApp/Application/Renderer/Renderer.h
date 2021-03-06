#pragma once

#include "RenderCommand.h"
#include "RendererAPI.h"
#include "Camera.h"
#include "Shader.h"
#include "SubTexture2D.h"
#include <memory>

namespace ChoreoApp {
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
            glm::mat4 viewProjectionMatrix;
        };
    private:

        static SceneData* m_sceneData;
        
    };

    // this class is super static - only a collection of render commands
    class Renderer2D {
    public:
        static void init();
        static void shutdown();
        static void beginScene(const OrthographicCamera& cam);
        static void flush();
        static void endScene();

        // primitives
        // TODO: Replace this with a properties struct and single function
        static void drawQuad(const glm::vec2& pos, const float angle,  const glm::vec2& size, const glm::vec4& color);
        static void drawQuad(const glm::vec3& pos,  const float angle, const glm::vec2& size, const glm::vec4& color);
        static void drawQuad(const glm::vec2& pos,  const float angle, const glm::vec2& size, const Ref<SubTexture2D>& tex);
        static void drawQuad(const glm::vec3& pos,  const float angle, const glm::vec2& size, const Ref<SubTexture2D>& tex);
        static void drawQuad(const glm::vec2& pos,  const float angle, const glm::vec2& size, const Ref<SubTexture2D>& tex, const glm::vec4& color);
        static void drawQuad(const glm::vec3& pos,  const float angle, const glm::vec2& size, const Ref<SubTexture2D>& tex, const glm::vec4& color);

        struct Stats{
            uint32_t drawCalls{0};
            uint32_t quadCount{0};

            uint32_t getTotalVertexCount() { return quadCount * 4; }
            uint32_t getTotalIndexCount() { return quadCount * 6; }
        }; 
        static void resetStats();
        static Stats getStats();

    private:
        static void resetCounters();
        static void startNewBatch();
        
    };
}
