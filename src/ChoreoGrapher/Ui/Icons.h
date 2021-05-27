#pragma once
#include "Application/Application.h"
#include "ChoreoApp.h"


namespace ChoreoGrapher{
    using IconRef =ChoreoApp::Ref<ChoreoApp::Texture2D> ;
        
    struct Icons{
        IconRef curveEditorIcon{ChoreoApp::Texture2D::create(
                ChoreoApp::Application::get().getRootDir() + 
                "/assets/icons/CurveEditor_32.png")};
    };

}
