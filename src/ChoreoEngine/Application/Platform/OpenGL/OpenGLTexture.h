#pragma once 


#include "Application/Renderer/Texture.h"

namespace ChoreoEngine {
    class OpenGLTexture2D : public Texture2D{
    public:
        OpenGLTexture2D(const std::string& path);

        virtual ~OpenGLTexture2D();

        virtual uint32_t  getWidth() const  override {return m_width;};
        virtual uint32_t  getHeight() const override {return m_height;};
        
        virtual void bind(uint32_t slot) const  override ;
    private:

       const std::string& m_path;

       uint32_t m_width{0};
       uint32_t m_height{0};

       uint32_t m_rendererId;
    };
}
