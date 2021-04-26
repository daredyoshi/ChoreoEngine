
#include <iostream>
#include "ChoreoApp.h"
#include "Application/EntryPoint.h"
#include "glm/fwd.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Layers.h"



class ChoreoGrapher : public ChoreoApp::Application{
public:
    ChoreoGrapher(const std::string& rootDir, const std::string& name) : Application(rootDir, name){
        // pushLayer( new 3DLayer() );
        pushLayer(new MainLayer());
    }

    ~ChoreoGrapher(){

    }
};

// overwrite the application creator to create sandbox
// entry point is in Application/EntryPoint.h

ChoreoApp::Application* ChoreoApp::CreateApplication(const std::string& rootDir){
    return new ChoreoGrapher(rootDir, "ChoreoGrapher");
}
