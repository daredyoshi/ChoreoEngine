#pragma once

#include "Core.h"
#include "Events/Event.h"

namespace ChoreoEngine {
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
        virtual void onUpdate() {}
        // layer recieves/intercepts event. 
        virtual void onEvent(Event& event);

        // don't use this to find the name
        inline const std::string& getName() const { return m_debugName; }
    protected:
        std::string m_debugName;
    };
}
