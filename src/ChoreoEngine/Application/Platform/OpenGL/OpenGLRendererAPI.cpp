#include "OpenGLRendererAPI.h"
#include "cepch.h"
#include <glad/glad.h>


void ChoreoEngine::OpenGLRendererAPI::clear() 
{
    // this is temporary because usually you don't want to clear both 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ChoreoEngine::OpenGLRendererAPI::drawIndexed(const Ref<VertexArray>& vertexArray) {
    glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
}

void ChoreoEngine::OpenGLRendererAPI::setClearColor(const glm::vec4& color) 
{
    glClearColor(color.r, color.g, color.b, color.a);
}

