#pragma once 

#include "Application/Renderer/GraphicsContext.h"

// so we don't need to include glfw
class GLFWwindow;

namespace ChoreoApp{
    class OpenGLContext : public GraphicsContext{
        
    public:
        OpenGLContext(GLFWwindow* windowHandle);
        virtual void Init() override;
        virtual void SwapBuffers() override;
    private:
        GLFWwindow* m_windowHandle;
    };
}
