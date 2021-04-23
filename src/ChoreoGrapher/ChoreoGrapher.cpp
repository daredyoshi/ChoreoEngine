
#include <iostream>
#include "ChoreoApp.h"
#include "Application/EntryPoint.h"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class MainLayer : public ChoreoApp::Layer{
public:
    MainLayer()
        : 
         Layer("Main") ,
         m_camController{1280.0f / 780.0f, true}
         {}

    virtual void onAttach() override {

    }

    virtual void onUpdate(ChoreoApp::TimeStep& timestep) override {
    }

    virtual void onEvent(ChoreoApp::Event& e) override{
        m_camController.onEvent(e);

        if (e.getEventType() == ChoreoApp::EventType::WindowResize){
                auto& re = (ChoreoApp::WindowResizeEvent&) e;
        }
    }
    
    virtual void onImGuiRender() override {
        ImGui::Begin("ChoreoApp::Settings");
        ImGui::End();
        ImGui::ShowDemoWindow();
    }


private:
    ChoreoApp::ShaderLibrary m_shaderLibrary;

    ChoreoApp::OrthographicCameraController m_camController;

    ChoreoApp::Ref<ChoreoApp::Texture2D> m_choreoLogo;
};

class ChoreoGrapher : public ChoreoApp::Application{
public:
    ChoreoGrapher(const std::string& rootDir, const std::string& name) : Application(rootDir, name){
        // pushLayer( new 3DLayer() );
        pushLayer(new MainLayer());
        pushOverlay(new ChoreoApp::ImGuiLayer());
    }

    ~ChoreoGrapher(){

    }
};

// overwrite the application creator to create sandbox
// entry point is in Application/EntryPoint.h

ChoreoApp::Application* ChoreoApp::CreateApplication(const std::string& rootDir){
    return new ChoreoGrapher(rootDir, "ChoreoGrapher");
}
