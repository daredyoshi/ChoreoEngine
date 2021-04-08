#include "cepch.h"
#include "LinuxInput.h"
#include "Application.h"

#include <GLFW/glfw3.h>

namespace ChoreoEngine {

    // becuase this is a fully static class we can just create
    // it here instead of an init
    Input* Input::s_instance = new LinuxInput();



	bool ChoreoEngine::LinuxInput::isKeyPressedImpl(int keycode){
        auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow() );
        auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool ChoreoEngine::LinuxInput::isMouseButtonPressedImpl(int button){
        auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow() );
        auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	float ChoreoEngine::LinuxInput::getMouseXImpl(){
        auto[x, y] = getMousePositionImpl(); 
        return x;
	}

	float ChoreoEngine::LinuxInput::getMouseYImpl(){
        auto[x, y] = getMousePositionImpl(); 
        return y;
	}

    std::pair<float, float> ChoreoEngine::LinuxInput::getMousePositionImpl(){
        auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow() );
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        return std::pair{xpos, ypos};
	}

}
