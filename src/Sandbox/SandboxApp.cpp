
#include <iostream>
#include "ChoreoEngine.h"

class ExampleLayer : public ChoreoEngine::Layer{
public:
    ExampleLayer()
        : Layer("Example"){}

    void onUpdate() override{

        if (ChoreoEngine::Input::isKeyPressed(CE_KEY_TAB)) 
            CE_INFO("Tab key is pressed");
    }

    void onEvent(ChoreoEngine::Event& event) override{
        (void)event;
        // CE_TRACE("{0}", event);
    }
};

class Sandbox : public ChoreoEngine::Application{
public:
    Sandbox(const std::string& name) : Application(name){
        pushLayer( new ExampleLayer() );
        pushOverlay(new ChoreoEngine::ImGuiLayer());
    }

    ~Sandbox(){

    }
};

ChoreoEngine::Application* ChoreoEngine::CreateApplication(){
    return new Sandbox("test");
}
