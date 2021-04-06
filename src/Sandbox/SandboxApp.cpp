
#include <iostream>
#include "ChoreoEngine.h"

class ExampleLayer : public ChoreoEngine::Layer{
public:
    ExampleLayer()
        : Layer("Example"){}

    void onUpdate() override{
        // CE_INFO("ExampleLayer::Update");
    }

    void onEvent(ChoreoEngine::Event& event) override{
        CE_TRACE("{0}", event);
    }
};

class Sandbox : public ChoreoEngine::Application{
public:
    Sandbox(const std::string& name) : Application(name){
        pushLayer( new ExampleLayer() );
    }

    ~Sandbox(){

    }
};

ChoreoEngine::Application* ChoreoEngine::CreateApplication(){
    return new Sandbox("test");
}
