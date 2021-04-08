#include "cepch.h"
#include "Layer.h"

namespace ChoreoEngine {

    Layer::Layer(const std::string& debugName) :
        m_debugName{debugName} {}

    Layer::~Layer(){}

    void Layer::onEvent(Event& event){
        (void)event;
    }
}
