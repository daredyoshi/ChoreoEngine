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

        virtual void onAttach() override;
        virtual void onDetach() override;
        virtual void onImGuiRender() override;


        void begin();
        void end();

    private:
        float m_time{0};
    };
}

