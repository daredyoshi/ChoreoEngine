
#include "ChoreGrapher.h"
// overwrite the application creator to create sandbox
// entry point is in Application/EntryPoint.h
#include "Application/EntryPoint.h"
// #include <iostream>
// #include "glm/fwd.hpp"
// #include "glm/gtc/type_ptr.hpp"
// #include "imgui.h"
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>



ChoreoApp::Application* ChoreoApp::CreateApplication(const std::string& rootDir){
    return new ChoreoGrapher::ChoreoGrapherApplication(rootDir, "ChoreoGrapher");    
}
