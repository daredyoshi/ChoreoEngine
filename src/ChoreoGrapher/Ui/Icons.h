#pragma once
#include "Application/Application.h"
#include "ChoreoApp.h"


namespace ChoreoGrapher{
    using IconRef =ChoreoApp::Ref<ChoreoApp::Texture2D> ;
        
    struct Icons{
        IconRef curveEditor{ChoreoApp::Texture2D::create(ChoreoApp::Application::get().getRootDir() + "/assets/icons/CurveEditor_32.png")};
        IconRef add{ChoreoApp::Texture2D::create(ChoreoApp::Application::get().getRootDir() + "/assets/icons/Add_32.png")};
        IconRef remove{ChoreoApp::Texture2D::create(ChoreoApp::Application::get().getRootDir() + "/assets/icons/Remove_32.png")};
        IconRef move{ChoreoApp::Texture2D::create(ChoreoApp::Application::get().getRootDir() + "/assets/icons/Move_32.png")};
    };

}
