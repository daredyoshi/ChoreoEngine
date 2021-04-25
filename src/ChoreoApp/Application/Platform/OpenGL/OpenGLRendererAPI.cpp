#include "OpenGLRendererAPI.h"
#include "capch.h"
#include <glad/glad.h>


void ChoreoApp::OpenGLRendererAPI::clear() 
{
    // this is temporary because usually you don't want to clear both 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ChoreoApp::OpenGLRendererAPI::drawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount) {
    if(indexCount == 0)
        indexCount = vertexArray->getIndexBuffer()->getCount();
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
}

void ChoreoApp::OpenGLRendererAPI::setClearColor(const glm::vec4& color) 
{
    glClearColor(color.r, color.g, color.b, color.a);
}


void ChoreoApp::OpenGLRendererAPI::init() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ChoreoApp::OpenGLRendererAPI::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) 
{
    glViewport(x, y, width, height);
}

