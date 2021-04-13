#include "cepch.h"
#include "OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace ChoreoEngine{
    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
        : m_windowHandle{windowHandle} {
        CE_CORE_ASSERT(windowHandle, "Window Handle is null!");
    }

	void ChoreoEngine::OpenGLContext::Init() 
	{
        // Set glfw to use open gl version 3.3 
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
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

	void ChoreoEngine::OpenGLContext::SwapBuffers() 
	{
        glfwSwapBuffers(m_windowHandle);
	}

}
