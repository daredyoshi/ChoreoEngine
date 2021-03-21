#pragma once

#include "Application.h"
#include <stdio.h>

// this will be implemented in the client using our library
extern ChoreoFlocking::Application* ChoreoFlocking::CreateApplication();

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;
    printf("Choreo Flocking \n");
    auto app = ChoreoFlocking::CreateApplication();

    app->run();
    delete app;
    return 0;
}
