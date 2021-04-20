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

    Ref<Shader> Shader::create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc){
        switch ( Renderer::getAPI() ){
            case RendererAPI::API::None: CE_CORE_ASSERT(false, "RendererAPI:None is currently not supported!");
            case RendererAPI::API::OpenGL: return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
        }

        CE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }


	Ref<Shader> ShaderLibrary::get(const std::string& name)
	{
        CE_CORE_ASSERT(exists(name), "Shader {0} not found!", name);
		return m_shaders[name];
	}

	Ref<Shader> ShaderLibrary::load(const std::string& name, const std::string& filepath)
	{
        auto shader = Shader::create(filepath);
        add(name, shader);
        return shader;
	}

	Ref<Shader> ShaderLibrary::load(const std::string& path)
	{
        auto shader = Shader::create(path);
        add(shader);
        return shader;
	}

	void ShaderLibrary::add(const std::string& name, const Ref<Shader>& shader)
	{
        CE_CORE_ASSERT(!exists(name), "Shader already exists!");
        m_shaders[name] = shader;
    }

	void ShaderLibrary::add(const Ref<Shader>& shader)
	{
        auto& name = shader->getName();
        add(name, shader);
	}

    bool ShaderLibrary::exists(const std::string& name) const {
        return m_shaders.find(name) != m_shaders.end();
    }


}
