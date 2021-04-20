#pragma once 


#include "Application/Renderer/Texture.h"

static const std::string s_tempPath {""};

namespace ChoreoEngine {
    class OpenGLTexture2D : public Texture2D{
    public:
        OpenGLTexture2D(const std::string& path);
        OpenGLTexture2D(uint32_t width, uint32_t height);
        

        virtual ~OpenGLTexture2D();

        virtual uint32_t  getWidth() const  override {return m_width;};
        virtual uint32_t  getHeight() const override {return m_height;};

        virtual void setdata(void* data, uint32_t size) override;
        
        virtual void bind(uint32_t slot) const  override ;
    private:

       const std::string& m_path{s_tempPath};

       uint32_t m_width{0};
       uint32_t m_height{0};

       uint32_t m_rendererId;
       unsigned int m_internalFormat, m_dataFormat;
    };
}
