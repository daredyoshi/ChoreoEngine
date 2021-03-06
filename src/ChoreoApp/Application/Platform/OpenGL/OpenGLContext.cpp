#include "capch.h"
#include "OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace ChoreoApp{
    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
        : m_windowHandle{windowHandle} {
        CE_CORE_ASSERT(windowHandle, "Window Handle is null!");
    }

	void ChoreoApp::OpenGLContext::Init() 
	{
        CE_PROFILE_FUNCTION ();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5); 
        // scale to monitor dpi
        glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        

        glfwMakeContextCurrent(m_windowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        CE_CORE_ASSERT(status, "Failed to initizlize Glad!");
        CE_CORE_INFO("Open GL Renderer:"); 
        CE_CORE_INFO("   Vendor {0}", glGetString(GL_VENDOR));
        CE_CORE_INFO("   Renderer {0}", glGetString(GL_RENDERER));
        CE_CORE_INFO("   Version {0}", glGetString(GL_VERSION));
	}

	void ChoreoApp::OpenGLContext::SwapBuffers() 
	{
        glfwSwapBuffers(m_windowHandle);
	}

}
