#include "capch.h"
#include "Application/Application.h"
#include "Application/Input.h"
#include <GLFW/glfw3.h>

namespace ChoreoApp {
	bool Input::isKeyPressed(int keycode){
        auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow() );
        auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::isMouseButtonPressed(int button){
        auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow() );
        auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	float Input::getMouseX(){
        auto[x, y] = getMousePosition(); 
        return x;
	}

	float Input::getMouseY(){
        auto[x, y] = getMousePosition(); 
        return y;
	}

    std::pair<float, float> Input::getMousePosition(){
        auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow() );
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        return std::pair{xpos, ypos};
	}

}
