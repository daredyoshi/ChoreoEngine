#pragma once

#include "Layer.h"
#include "Events/MouseEvent.h"
#include "Events/Application.h"
#include "Events/KeyEvent.h"

namespace ChoreoEngine {
    class ImGuiLayer: public Layer{

    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void onUpdate();
        void onEvent(Event& event);
        void onAttach();
        void onDetach();

    private:
        bool onMouseButtonPressedEvent(MouseButtonPressedEvent& e);
        bool onMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
        bool onMouseMovedEvent(MouseMovedEvent& e);
        bool onMouseScrolledEvent(MouseScrollEvent& e);
        bool onKeyReleasedEvent(KeyReleasedEvent& e);
        bool onKeyPressedEvent(KeyPressedEvent& e);
        bool onKeyTypedEvent(KeyTypedEvent& e);
        bool onWindowResizeEvent(WindowResizeEvent& e);



        float m_time{0};
    };
}

