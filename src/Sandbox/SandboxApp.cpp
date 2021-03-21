
#include "ChoreoFlocking/Application.h"
#include <iostream>
#include <ChoreoFlocking.h>

class Sandbox : public ChoreoFlocking::Application{
public:
    Sandbox(){
    }

    ~Sandbox(){

    }
};

ChoreoFlocking::Application* ChoreoFlocking::CreateApplication(){
    return new Sandbox();
}
