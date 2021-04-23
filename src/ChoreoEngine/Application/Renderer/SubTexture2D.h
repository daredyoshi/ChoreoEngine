#pragma once 


#include <glm/glm.hpp>
#include "Texture.h"

namespace ChoreoEngine{
    class SubTexture2D{
    public:
        SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);


        const Ref<Texture2D> getTexture() const { return m_texture; }
        const glm::vec2* getUvs() const { return m_uvs; }

        static Ref<SubTexture2D> createFromCoords(const Ref<Texture2D>& texture , const glm::vec2 spriteUnitCoords, const glm::vec2 spriteUnitDimensions, const unsigned int spriteUnitPixels);

        virtual bool operator==(const SubTexture2D& other) const { return m_texture == other.m_texture;};
    private:
        const Ref<Texture2D> m_texture;
        glm::vec2 m_uvs[4];
    };
}

