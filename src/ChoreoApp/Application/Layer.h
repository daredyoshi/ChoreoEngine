#pragma once

#include "Application/Core.h"
#include "Application/Events/Event.h"
#include "Application/Timestep.h"

namespace ChoreoApp {
    // This class is intended to be inherited from so that 
    // every layer is unique
    // What this doesn't have is enabling/disabling layers
    class Layer{
    public:
        Layer( const std::string& name = "Layer" );
        virtual ~Layer();
        
        // layer gets pushed onto layer stack
        virtual void onAttach() {}
        // layer removed from layer stack
        virtual void onDetach() {}
        // layer updated (per frame)
        virtual void onUpdate(Timestep& timestep) {(void)timestep;}
        // imgui render layer
        virtual void onImGuiRender() {}
        // layer recieves/intercepts event. 
        virtual void onEvent(Event& event);

        // don't use this to find the name
        inline const std::string& getName() const { return m_debugName; }
    protected:
        std::string m_debugName;
    };
}
