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
        virtual void onImGuiRender() override;


        void begin();
        void end();

    private:
        float m_time{0};
    };
}

