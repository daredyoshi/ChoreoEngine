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

ChoreoEngine::Ref<ChoreoEngine::SubTexture2D> ChoreoEngine::SubTexture2D::createFromCoords(const Ref<Texture2D>& texture , const glm::vec2 spriteUnitCoords, const glm::vec2 spriteUnitDimensions, const unsigned int spriteUnitPixels){
    CE_CORE_TRACE(" texture dimensions {0}, {1}", texture->getWidth(), texture->getHeight());
    glm::vec2 min = { ( spriteUnitCoords.x * spriteUnitPixels ) / texture->getWidth(), ( spriteUnitCoords.y * spriteUnitPixels ) / texture->getHeight()};
    glm::vec2 max = { ( (spriteUnitCoords.x + spriteUnitDimensions.x) * spriteUnitPixels ) / texture->getWidth(), ((spriteUnitCoords.y + spriteUnitDimensions.y) * spriteUnitPixels) / texture->getHeight()};
	return CreateRef<SubTexture2D>(texture, min, max);
}

