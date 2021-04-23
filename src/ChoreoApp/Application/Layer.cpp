#include "capch.h"
#include "Application/Layer.h"

namespace ChoreoApp {

    Layer::Layer(const std::string& debugName) :
        m_debugName{debugName} {}

    Layer::~Layer(){}

    void Layer::onEvent(Event& event){
        (void)event;
    }
}
