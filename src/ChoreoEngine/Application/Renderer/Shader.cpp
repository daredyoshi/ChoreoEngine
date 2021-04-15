#include "cepch.h"
#include "Application/Renderer/Shader.h"
#include "Application/Renderer/Renderer.h"
#include "glad/glad.h"
#include "Application/Platform/OpenGL/OpenGLShader.h"


namespace ChoreoEngine{

    Ref<Shader> Shader::create(const std::string& path){
        switch ( Renderer::getAPI() ){
            case RendererAPI::API::None: CE_CORE_ASSERT(false, "RendererAPI:None is currently not supported!");
            case RendererAPI::API::OpenGL: return CreateRef<OpenGLShader>(path);
        }

        CE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
        
    }

    Ref<Shader> Shader::create(const std::string& vertexSrc, const std::string& fragmentSrc){
        switch ( Renderer::getAPI() ){
            case RendererAPI::API::None: CE_CORE_ASSERT(false, "RendererAPI:None is currently not supported!");
            case RendererAPI::API::OpenGL: return CreateRef<OpenGLShader>(vertexSrc, fragmentSrc);
        }

        CE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}
