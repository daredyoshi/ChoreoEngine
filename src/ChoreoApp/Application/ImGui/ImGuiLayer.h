#pragma once

#include "Application/Layer.h"
#include "Application/Events/MouseEvent.h"
#include "Application/Events/Application.h"
#include "Application/Events/KeyEvent.h"

namespace ChoreoApp {
    class ImGuiLayer: public Layer{

    public:
        ImGuiLayer();
        ~ImGuiLayer();

        virtual void onAttach() override;
        virtual void onDetach() override;
        virtual void onEvent(Event& ) override;


        void begin();
        void end();

        void setConsumeImGuiEvents( bool consume) { m_consumeImGuiEvents = consume; }

    private:
        bool m_consumeImGuiEvents{true};
        float m_time{0};
    };
}

