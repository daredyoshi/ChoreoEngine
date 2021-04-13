#include "cepch.h"
#include "Renderer.h"
#include "RenderCommand.h"

namespace ChoreoEngine{

	void Renderer::beginScene()
	{
	}

	void Renderer::endScene()
	{
	}

	void Renderer::submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
        vertexArray->bind();
        RenderCommand::DrawIndexed(vertexArray);
	}

}
