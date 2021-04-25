#include "capch.h"
#include "Renderer.h"
#include "RenderCommand.h"
#include "VertexArray.h"
#include "Shader.h"
#include <memory>

// temp 
#include "Application/Application.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/trigonometric.hpp"

namespace ChoreoApp{

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
        float textureIndex;
        float tilingFactor;
    };
    
    struct Renderer2DStorage{
        // these are the limits for each batched draw call
        // if it goes above this we will simply do more draw calls per frame
        // but with the maxQuads per draw call
        static const uint32_t maxQuads = 10000;
        static const uint32_t maxVertices = maxQuads * 4;
        static const uint32_t maxIndices = maxQuads * 6;
        static const uint32_t maxTextureSlots = 32; // TODO: Render Caps

        // should probably be Scope isntead
        Ref<VertexArray> quadVertexArray;
        Ref<VertexBuffer> quadVertexBuffer;
        Ref<Shader> shader2D;
        Ref<Texture2D> emptyTex;
        Ref<SubTexture2D> emptySubTex;

        // the runtime amount of quads to draw
        uint32_t quadIndexCount = 0;

        QuadVertex* quadVertexBufferBase = nullptr; 
        QuadVertex* quadVertexBufferPtr= nullptr; 

        // we are using opengl ids, but in the future this
        // should be the asset id
        std::array<Ref<SubTexture2D>, maxTextureSlots> textureSlots;
        uint32_t textureSlotIndex = 1; // 0 is the white empty texture

        glm::vec4 quadVertexPositions[4];

        Renderer2D::Stats stats;
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
            { ShaderDataType::Float2, "a_Uv" },
            { ShaderDataType::Float, "a_TexIdx" },
            { ShaderDataType::Float, "a_TilingFactor" }
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

        s_storage.emptySubTex = CreateRef<SubTexture2D>( s_storage.emptyTex, glm::vec2{0.0f, 0.0f}, glm::vec2{1.0f, 1.0f} );

        const std::string shaderSrcPath{Application::get().getRootDir() + "assets/shaders/QuadShader.glsl"} ;
        CE_INFO("Texture Shader Source Path = {0}",shaderSrcPath);
        s_storage.shader2D= Shader::create(shaderSrcPath);
        s_storage.shader2D->bind();
        // bind ids 0 - 32
        int32_t samplers[s_storage.maxTextureSlots];
        for (uint32_t i{0}; i<s_storage.maxTextureSlots; ++i){
            samplers[i] = i;
        }
        s_storage.shader2D->setIntArray("u_textures", samplers, s_storage.maxTextureSlots);

        // initialize all the slots to 0 
        s_storage.textureSlots[0] = s_storage.emptySubTex;

        s_storage.quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        s_storage.quadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
        s_storage.quadVertexPositions[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
        s_storage.quadVertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };
    }

    void Renderer2D::shutdown(){
        CE_PROFILE_FUNCTION();  
    }

    void Renderer2D::beginScene(const OrthographicCamera& cam){ 
        CE_PROFILE_FUNCTION();  

        s_storage.shader2D->bind();
        s_storage.shader2D->setMat4("u_viewProjection", cam.getViewProjectionMatrix());
        resetCounters();
    }

    void Renderer2D::flush(){
        CE_PROFILE_FUNCTION();  
        s_storage.shader2D->bind();
        s_storage.shader2D->setFloat("u_tilingFactor", 1.0f);

        for (uint32_t i{0}; i<s_storage.textureSlotIndex; i++){
            s_storage.textureSlots[i]->getTexture()->bind(i);
        }

        s_storage.quadVertexArray->bind();
        RenderCommand::DrawIndexed(s_storage.quadVertexArray, s_storage.quadIndexCount);
        s_storage.stats.drawCalls++;
    }

    void Renderer2D::endScene(){
        CE_PROFILE_FUNCTION();  
        // cast to uint8_t only because that's one byte and we're getting the size
        // based on their difference
        uint32_t dataSize = (uint8_t*)s_storage.quadVertexBufferPtr - (uint8_t*)s_storage.quadVertexBufferBase;
        s_storage.quadVertexBuffer->setData(s_storage.quadVertexBufferBase, dataSize);
        flush();
    }

    void Renderer2D::resetCounters(){
        s_storage.quadIndexCount = 0;
        s_storage.quadVertexBufferPtr = s_storage.quadVertexBufferBase;
        s_storage.textureSlotIndex = 1;
    }

    void Renderer2D::startNewBatch(){
        CE_PROFILE_FUNCTION();
        Renderer2D::endScene();
        Renderer2D::resetCounters();
    }


    // default z to 0
    void Renderer2D::drawQuad(const glm::vec2& pos, const float angleInRadians, const glm::vec2& size, const glm::vec4& color){
        drawQuad({pos.x, pos.y, 0.0f}, angleInRadians, size, s_storage.emptySubTex, color);
    }

    void Renderer2D::drawQuad(const glm::vec3& pos, const float angleInRadians, const glm::vec2& size, const glm::vec4& color){
        drawQuad( pos, angleInRadians, size, s_storage.emptySubTex, color );
    }
    void Renderer2D::drawQuad(const glm::vec2& pos,  const float angleInRadians, const glm::vec2& size, const Ref<SubTexture2D>& subTex){
        drawQuad({pos.x, pos.y, 0.0f}, angleInRadians, size, subTex, glm::vec4{1});
    }
    void Renderer2D::drawQuad(const glm::vec3& pos,  const float angleInRadians, const glm::vec2& size, const Ref<SubTexture2D>& subTex){
        drawQuad(pos, angleInRadians, size, subTex, glm::vec4{1});
    }
    void Renderer2D::drawQuad(const glm::vec2& pos,  const float angleInRadians, const glm::vec2& size, const Ref<SubTexture2D>& subTex, const glm::vec4& color){
        drawQuad({pos.x, pos.y, 0.0f}, angleInRadians, size, subTex, color);
    }

    void Renderer2D::drawQuad(const glm::vec3& pos,  const float angleInRadians, const glm::vec2& size, const Ref<SubTexture2D>& subTex, const glm::vec4& color){
        CE_PROFILE_FUNCTION();  

        if(s_storage.quadIndexCount >= Renderer2DStorage::maxIndices){
            startNewBatch();
        }

        float tilingFactor = 1.0;
        float textureIndex = 0;




        // if the tex is not the empty tex find it's id
        if (!( *subTex.get() == *s_storage.emptySubTex.get() )){
            // check if textuer index in indices here
            for (uint32_t i{0}; i< s_storage.textureSlotIndex; i++){
                // replace this with an asset comapre
                if(*s_storage.textureSlots[i].get() == *subTex.get()){
                    textureIndex = i; 
                    break;
                }
            }

            if (textureIndex == 0){
                textureIndex = (float)s_storage.textureSlotIndex;
                s_storage.textureSlots[s_storage.textureSlotIndex] = subTex;
                s_storage.textureSlotIndex++;
            }
        }


        // figure out rotation
        glm::mat4 xform = glm::translate(glm::mat4(1.0f), pos);
        xform *= glm::rotate(glm::mat4(1.0f), angleInRadians, glm::vec3{0, 0, 1});
        xform *= glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0});

        for(unsigned int i{0}; i<4; ++i){
            // vertex 1 
            s_storage.quadVertexBufferPtr->position = xform * s_storage.quadVertexPositions[i];
            s_storage.quadVertexBufferPtr->color = color;
            s_storage.quadVertexBufferPtr->uv= subTex->getUvs()[i];
            s_storage.quadVertexBufferPtr->textureIndex= textureIndex;
            s_storage.quadVertexBufferPtr->tilingFactor= tilingFactor;
            ++s_storage.quadVertexBufferPtr;

        }

        s_storage.quadIndexCount+=6;
        s_storage.stats.quadCount++;

    }

    void Renderer2D::resetStats(){
        s_storage.stats.drawCalls = 0;
        s_storage.stats.quadCount= 0;
    }

    Renderer2D::Stats Renderer2D::getStats() {
        return s_storage.stats;
    }

    
}

