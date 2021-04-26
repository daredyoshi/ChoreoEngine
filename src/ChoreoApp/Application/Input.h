#pragma once

#include <utility>
#include "Application/Core.h"


namespace ChoreoApp {

    // for now these don't have our own keycodes so we are 
    // using the glfw ones 
    class Input{
    public:
        static bool isKeyPressed(int keycode); 
        static bool isMouseButtonPressed(int button); 
        static std::pair<float , float> getMousePosition();
        static float getMouseX(); 
        static float getMouseY();
    };

}
