#include "cepch.h"
#include "Renderer.h"
#include "RenderCommand.h"
#include "Application/Platform/OpenGL/OpenGLShader.h"
#include <memory>

namespace ChoreoEngine{

    Renderer::SceneData* Renderer::m_sceneData = new Renderer::SceneData;

	void Renderer::beginScene(OrthographicCamera& cam){
        m_sceneData->viewPrjectionMatrix = cam.getViewProjectionMatrix();
	}

	void Renderer::endScene(){
	}

	void Renderer::submit(const Ref<Shader>& shader, 
            const Ref<VertexArray>& vertexArray, 
            const glm::mat4& xform){
        vertexArray->bind();
        shader->bind();
        std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniformMat4("u_viewProjection", m_sceneData->viewPrjectionMatrix);
        std::dynamic_pointer_cast<OpenGLShader>(shader)->uploadUniformMat4("u_xform", xform);
        RenderCommand::DrawIndexed(vertexArray);
	}

    void Renderer::init(){
        RenderCommand::init();
    }

}
