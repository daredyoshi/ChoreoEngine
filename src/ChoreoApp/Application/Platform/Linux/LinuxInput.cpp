#include "Application/Platform/Linux/LinuxInput.h"
#include "Application/Application.h"

#include <GLFW/glfw3.h>

namespace ChoreoApp {

    // becuase this is a fully static class we can just create
    // it here instead of an init
    Input* Input::s_instance = new LinuxInput();



	bool ChoreoApp::LinuxInput::isKeyPressedImpl(int keycode){
        auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow() );
        auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool ChoreoApp::LinuxInput::isMouseButtonPressedImpl(int button){
        auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow() );
        auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	float ChoreoApp::LinuxInput::getMouseXImpl(){
        auto[x, y] = getMousePositionImpl(); 
        return x;
	}

	float ChoreoApp::LinuxInput::getMouseYImpl(){
        auto[x, y] = getMousePositionImpl(); 
        return y;
	}

    std::pair<float, float> ChoreoApp::LinuxInput::getMousePositionImpl(){
        auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow() );
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        return std::pair{xpos, ypos};
	}

}
