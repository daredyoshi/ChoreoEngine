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
        CE_PROFILE_FUNCTION();  
        RenderCommand::init();
        Renderer2D::init();
    }

    void Renderer::onWindowResize(uint32_t width, uint32_t height){
        RenderCommand::setViewport(0, 0, width, height);
    }


    /////////////// RENDERER 2D //////////////////
    //////////////////////////////////////////////
    struct QuadVertex{
        glm::vec3 position;
        glm::vec4 color;
        glm::vec2 uv;
    };
    
    struct Renderer2DStorage{
        // these are the limits for each batched draw call
        // if it goes above this we will simply do more draw calls per frame
        // but with the maxQuads per draw call
        const uint32_t maxQuads = 10000;
        const uint32_t maxVertices = maxQuads * 4;
        const uint32_t maxIndices = maxQuads * 6;

        // should probably be Scope isntead
        Ref<VertexArray> quadVertexArray;
        Ref<VertexBuffer> quadVertexBuffer;
        Ref<Shader> shader2D;
        Ref<Texture2D> emptyTex;

        // the runtime amount of quads to draw
        uint32_t quadIndexCount = 0;

        QuadVertex* quadVertexBufferBase = nullptr; 
        QuadVertex* quadVertexBufferPtr= nullptr; 
    };


    static Renderer2DStorage s_storage; 

    void Renderer2D::init(){
        CE_PROFILE_FUNCTION();  
        // square test
        
        s_storage.quadVertexArray = VertexArray::create();
        float squareVertices[ 5 * 4] = {
            // positions                
            -0.5f,  -0.5f,   0.0f,   0.0f, 0.0f,      
             0.5f,  -0.5f,   0.0f,   1.0f, 0.0f,         
             0.5f,   0.5f,   0.0f,   1.0f, 1.0f,        
            -0.5f,   0.5f,   0.0f,   0.0f, 1.0f        
        };

        s_storage.quadVertexBuffer = VertexBuffer::create(s_storage.maxVertices * sizeof(QuadVertex));
        
        s_storage.quadVertexBuffer->setLayout({
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float4, "a_Color" },
            { ShaderDataType::Float2, "a_Uv" }
        });
        s_storage.quadVertexArray->addVertexBuffer(s_storage.quadVertexBuffer);

        s_storage.quadVertexBufferBase = new QuadVertex[s_storage.maxVertices];

        // index buffer
        // allocating on heap instead we dramatically increase maxQuads
        // This is bad because we are assuming that this will get immediately 
        // uploaded to the gpu which is not always the case
        uint32_t* quadIndices = new uint32_t[s_storage.maxIndices];

        // fill all the indices
        uint32_t offset = 0;
        for (uint32_t i = 0; i < s_storage.maxIndices; i += 6){
            // triangle a
            quadIndices[i+0] = offset + 0;
            quadIndices[i+1] = offset + 1;
            quadIndices[i+2] = offset + 2;
            // triangle b
            quadIndices[i+3] = offset + 2;
            quadIndices[i+4] = offset + 3;
            quadIndices[i+5] = offset + 0;

            offset +=4;
        }

        Ref<IndexBuffer> quadIB;
        // would be great to set the update type to dynamic somehow...
        quadIB = IndexBuffer::create(quadIndices, s_storage.maxIndices);
        s_storage.quadVertexArray->setIndexBuffer(quadIB);

        delete[] quadIndices;

        // create 1x1 white texture
        s_storage.emptyTex = Texture2D::create(1, 1);
        uint32_t whiteTexData = 0xffffffff;
        s_storage.emptyTex->setdata(&whiteTexData, sizeof(whiteTexData));


        const std::string shaderSrcPath{Application::get().getRootDir() + "assets/shaders/QuadShader.glsl"} ;
        CE_INFO("Texture Shader Source Path = {0}",shaderSrcPath);
        s_storage.shader2D= Shader::create(shaderSrcPath);
        s_storage.shader2D->bind();
        s_storage.shader2D->setInt("u_texture", 0);
    }

    void Renderer2D::shutdown(){
        CE_PROFILE_FUNCTION();  
    }

    void Renderer2D::beginScene(const OrthographicCamera& cam){ 
        CE_PROFILE_FUNCTION();  

        s_storage.shader2D->bind();
        s_storage.shader2D->setMat4("u_viewProjection", cam.getViewProjectionMatrix());

        s_storage.quadIndexCount = 0;
        // now we can increment this without ofsetting base 
        s_storage.quadVertexBufferPtr = s_storage.quadVertexBufferBase;
    }

    void Renderer2D::flush(){
        CE_PROFILE_FUNCTION();  
        s_storage.shader2D->bind();
        s_storage.shader2D->setFloat("u_tilingFactor", 1.0f);
        // tex->bind(0);
        
        // change this to not being done on the shader
        // s_storage.shader2D->setMat4("u_xform", xform);

        s_storage.quadVertexArray->bind();
        RenderCommand::DrawIndexed(s_storage.quadVertexArray, s_storage.quadIndexCount);

    }

    void Renderer2D::endScene(){
        CE_PROFILE_FUNCTION();  
        // cast to uint8_t only because that's one byte and we're getting the size
        // based on their difference
        uint32_t dataSize = (uint8_t*)s_storage.quadVertexBufferPtr - (uint8_t*)s_storage.quadVertexBufferBase;
        s_storage.quadVertexBuffer->setData(s_storage.quadVertexBufferBase, dataSize);
        flush();
    }


    // default z to 0
    void Renderer2D::drawQuad(const glm::vec2& pos, const float angle, const glm::vec2& size, const glm::vec4& color){
        drawQuad({pos.x, pos.y, 0.0f}, angle, size, s_storage.emptyTex, color);
    }

    void Renderer2D::drawQuad(const glm::vec3& pos, const float angle, const glm::vec2& size, const glm::vec4& color){
        drawQuad( pos, angle, size, s_storage.emptyTex, color );
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
        (void)tex;
        (void)angle;
        CE_PROFILE_FUNCTION();  

        // glm::vec3 xformPos = glm::vec3(glm::vec4(pos, 1) * glm::mat4 {  
        //                 glm::scale(
        //                     glm::rotate( 
        //                         glm::translate(
        //                             glm::mat4{ 1.0f }, 
        //                             pos), 
        //                     glm::radians(angle), 
        //                     glm::vec3{0.0f, 0.0f, 1.0f}
        //                     ), 
        //                 glm::vec3{size.x, size.y, 1.0} 
        //             ),
        // });
        // vertex 1 
        s_storage.quadVertexBufferPtr->position = pos;
        s_storage.quadVertexBufferPtr->color = color;
        s_storage.quadVertexBufferPtr->uv= {0.0f, 0.0f};
        s_storage.quadVertexBufferPtr++;
        // vertex 2 
        s_storage.quadVertexBufferPtr->position = {pos.x + size.x, pos.y, pos.z};
        s_storage.quadVertexBufferPtr->color = color;
        s_storage.quadVertexBufferPtr->uv= {1.0f, 0.0f};
        s_storage.quadVertexBufferPtr++;
        // vertex 3 
        s_storage.quadVertexBufferPtr->position = {pos.x + size.x, pos.y + size.y, pos.z};
        s_storage.quadVertexBufferPtr->color = color;
        s_storage.quadVertexBufferPtr->uv= {1.0f, 1.0f};
        s_storage.quadVertexBufferPtr++;
        // vertex 4 
        s_storage.quadVertexBufferPtr->position = {pos.x, pos.y + size.y, pos.z};
        s_storage.quadVertexBufferPtr->color = color;
        s_storage.quadVertexBufferPtr->uv= {0.0f, 1.0f};
        s_storage.quadVertexBufferPtr++;

        s_storage.quadIndexCount+=6;

    }
}

