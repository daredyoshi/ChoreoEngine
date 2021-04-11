
#include <iostream>
#include "ChoreoEngine.h"
#include "imgui.h"

class ExampleLayer : public ChoreoEngine::Layer{
public:
    ExampleLayer()
        : Layer("Example"){}

    
    virtual void onImGuiRender() override {
        ImGui::Begin("ChoreoGrapher::Test");
        ImGui::Text("HelloWorld");
        ImGui::End();
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
    return new Sandbox("ChoreoGrapher");
}
