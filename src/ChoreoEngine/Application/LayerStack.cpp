#include "cepch.h"
#include "Layer.h"
#include "LayerStack.h"


namespace ChoreoEngine {

    LayerStack::LayerStack(){
        m_layerInsert = m_layers.begin();
    }

    // the layer stack destroys all active layer 
    // pointers if it is destroyed. This only 
    // happens if the application closes. 
    LayerStack::~LayerStack(){
        for(Layer* layer: m_layers)
            delete layer;
    }

    void LayerStack::pushLayer(Layer* layer){
        m_layerInsert = m_layers.emplace(m_layerInsert, layer);
    }

    // overlays always render last
    void LayerStack::pushOverlay(Layer* overlay){
        m_layers.emplace_back(overlay);
    }

    // layers are not destroyed when taken off the stack
    void LayerStack::popLayer(Layer* layer){
        auto it = std::find(m_layers.begin(), m_layers.end(), layer);
        if (it != m_layers.end()){
            m_layers.erase(it);
            m_layerInsert--;
        }
    }

    // overlays always render last
    void LayerStack::popOverlay(Layer* overlay){
        auto it = std::find(m_layers.begin(), m_layers.end(), overlay);
        if (it != m_layers.end())
            m_layers.erase(it);
    }
}
