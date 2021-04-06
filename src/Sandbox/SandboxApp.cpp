
#include <iostream>
#include "ChoreoEngine.h"


class Sandbox : public ChoreoEngine::Application{
public:
    Sandbox(const std::string& name) : Application(name){
    }

    ~Sandbox(){

    }
};

ChoreoEngine::Application* ChoreoEngine::CreateApplication(){
    return new Sandbox("test");
}
