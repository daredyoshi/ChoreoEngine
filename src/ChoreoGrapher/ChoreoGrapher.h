#pragma once

#include "ChoreoApp.h"
#include "Ui/EditorLayer.h"

namespace ChoreoGrapher{

class ChoreoGrapherApplication : public ChoreoApp::Application{
public:
    ChoreoGrapherApplication(const std::string& rootDir, const std::string& name) : Application(rootDir, name){
        // pushLayer( new 3DLayer() );
        setFontSize(23);
        pushLayer(new EditorLayer());
    }

    ~ChoreoGrapherApplication(){

    }

    const Icons& getIcons() const { return m_icons; }
    uint32_t getFontSize() const { return m_fontSize; }
    void setFontSize(uint32_t size);

    ImFont* getFont() const { return m_font; }


private:

    // font
    ImFont* m_font = nullptr; 
    std::string fontFile = (ChoreoApp::Application::get().getRootDir() + "assets/fonts/droid-sans/DroidSans.ttf");
    uint32_t m_fontSize;
    Icons m_icons{};
};
}
