#pragma once

#include "ChoreoApp.h"
#include "Ui/EditorLayer.h"

namespace ChoreoGrapher{

class ChoreoGrapherApplication : public ChoreoApp::Application{
public:
    ChoreoGrapherApplication(const std::string& rootDir, const std::string& name) : Application(rootDir, name){
        // pushLayer( new 3DLayer() );
        pushLayer(new EditorLayer());
    }

    ~ChoreoGrapherApplication(){

    }

    const Icons& getIcons() const { return m_icons; }

private:
    Icons m_icons{};
};
}
