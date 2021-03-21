
#include <iostream>
#include "ChoreoEngine.h"

class Sandbox : public ChoreoEngine::Application{
public:
    Sandbox(){
    }

    ~Sandbox(){

    }
};

ChoreoEngine::Application* ChoreoEngine::CreateApplication(){
    return new Sandbox();
}
