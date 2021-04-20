#include "cepch.h"
#include "Renderer.h"
#include "RenderCommand.h"
#include "VertexArray.h"
#include "Shader.h"
#include <memory>

// temp 
#include "Application/Application.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"

namespace ChoreoEngine{

    Renderer::SceneData* Renderer::m_sceneData = new Renderer::SceneData;

	void Renderer::beginScene(OrthographicCamera& cam){
        m_sceneData->viewProjectionMatrix = cam.getViewProjectionMatrix();
	}

	void Renderer::endScene(){
	}

	void Renderer::submit(const Ref<Shader>& shader, 
            const Ref<VertexArray>& vertexArray, 
            const glm::mat4& xform){
        vertexArray->bind();
        shader->bind();
        shader->setMat4("u_viewProjection", m_sceneData->viewProjectionMatrix);
        shader->setMat4("u_xform", xform);
        RenderCommand::DrawIndexed(vertexArray);
	}

    void Renderer::init(){
        RenderCommand::init();
        Renderer2D::init();
    }

    void Renderer::onWindowResize(uint32_t width, uint32_t height){
        RenderCommand::setViewport(0, 0, width, height);
    }


    /////////////// RENDERER 2D //////////////////
    //////////////////////////////////////////////
    
    struct Renderer2DStorage{
        // should probably be Scope isntead
        Ref<VertexArray> quadVertexArray;
        Ref<Shader> shader2D;
        Ref<Texture2D> emptyTex;
    };

    static Renderer2DStorage* s_storage; 

    void Renderer2D::init(){
        // square test
        s_storage = new Renderer2DStorage();
        
        s_storage->quadVertexArray = VertexArray::create();
        float squareVertices[ 5 * 4] = {
            // positions                
            -0.5f,  -0.5f,   0.0f,   0.0f, 0.0f,      
             0.5f,  -0.5f,   0.0f,   1.0f, 0.0f,         
             0.5f,   0.5f,   0.0f,   1.0f, 1.0f,        
            -0.5f,   0.5f,   0.0f,   0.0f, 1.0f        
        };

        Ref<VertexBuffer> squareVB;
        squareVB = VertexBuffer::create(squareVertices, sizeof(squareVertices));
        
        squareVB->setLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float2, "a_Uv" }
        });
        s_storage->quadVertexArray->addVertexBuffer(squareVB);

        // index buffer
        uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
        Ref<IndexBuffer> squareIB;
        squareIB = IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
        s_storage->quadVertexArray->setIndexBuffer(squareIB);

        // create 1x1 white texture
        s_storage->emptyTex = Texture2D::create(1, 1);
        uint32_t whiteTexData = 0xffffffff;
        s_storage->emptyTex->setdata(&whiteTexData, sizeof(whiteTexData));


        const std::string shaderSrcPath{Application::get().getRootDir() + "assets/shaders/QuadShader.glsl"} ;
        CE_INFO("Texture Shader Source Path = {0}",shaderSrcPath);
        s_storage->shader2D= Shader::create(shaderSrcPath);
        s_storage->shader2D->bind();
        s_storage->shader2D->setInt("u_texture", 0);
    }

    void Renderer2D::shutdown(){
        delete s_storage;
    }

    void Renderer2D::beginScene(const OrthographicCamera& cam){ 
        s_storage->shader2D->bind();
        s_storage->shader2D->setMat4("u_viewProjection", cam.getViewProjectionMatrix());
    }

    void Renderer2D::endScene(){

    }


    // default z to 0
    void Renderer2D::drawQuad(const glm::vec2& pos, const float angle, const glm::vec2& size, const glm::vec4& color){
        drawQuad({pos.x, pos.y, 0.0f}, angle, size, s_storage->emptyTex, color);
    }

    void Renderer2D::drawQuad(const glm::vec3& pos, const float angle, const glm::vec2& size, const glm::vec4& color){
        drawQuad( pos, angle, size, s_storage->emptyTex, color );
    }
    void Renderer2D::drawQuad(const glm::vec2& pos,  const float angle, const glm::vec2& size, const Ref<Texture2D>& tex){
        drawQuad({pos.x, pos.y, 0.0f}, angle, size, tex, glm::vec4{1});
    }
    void Renderer2D::drawQuad(const glm::vec3& pos,  const float angle, const glm::vec2& size, const Ref<Texture2D>& tex){
        drawQuad(pos, angle, size, tex, glm::vec4{1});
    }
    void Renderer2D::drawQuad(const glm::vec2& pos,  const float angle, const glm::vec2& size, const Ref<Texture2D>& tex, const glm::vec4& color){
        drawQuad({pos.x, pos.y, 0.0f}, angle, size, tex, color);
    }

    void Renderer2D::drawQuad(const glm::vec3& pos,  const float angle, const glm::vec2& size, const Ref<Texture2D>& tex, const glm::vec4& color){
        s_storage->shader2D->bind();
        s_storage->shader2D->setFloat4("u_color", color);
        tex->bind(0);
        
        glm::mat4 xform {  
                        glm::scale(
                            glm::rotate( 
                                glm::translate(
                                    glm::mat4{ 1.0f }, 
                                    pos), 
                            glm::radians(angle), 
                            glm::vec3{0.0f, 0.0f, 1.0f}
                            ), 
                        glm::vec3{size.x, size.y, 1.0} 
                    ),
        };
        s_storage->shader2D->setMat4("u_xform", xform);

        s_storage->quadVertexArray->bind();
        RenderCommand::DrawIndexed(s_storage->quadVertexArray);

    }
}

