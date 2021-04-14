#pragma once

#include "Application/Renderer/RendererAPI.h"

namespace ChoreoEngine {
    class OpenGLRendererAPI : public RendererAPI {
    public:

        virtual void clear() override;
        virtual void setClearColor(const glm::vec4& color) override;
        virtual void drawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
    };
}
