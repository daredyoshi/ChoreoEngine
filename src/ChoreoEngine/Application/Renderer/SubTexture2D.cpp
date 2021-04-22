#include "cepch.h"
#include "SubTexture2D.h"

ChoreoEngine::SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
    :
        m_texture{texture}        
{
    m_uvs[0] = {min.x, min.y}; 
    m_uvs[1] = {max.x, min.y}; 
    m_uvs[2] = {max.x, max.y}; 
    m_uvs[3] = {min.x, max.y}; 
}

ChoreoEngine::Ref<ChoreoEngine::SubTexture2D> ChoreoEngine::SubTexture2D::createFromCoords(glm::vec2 spriteIdx, glm::vec2 spriteSize)
{
    const 

    glm::vec2 min = { {spriteIdx.x * spriteSize.x}, spriteSize }
    
	return SubTexture2D{};
}

