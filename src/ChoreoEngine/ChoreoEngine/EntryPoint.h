#pragma once

#include "Application.h"
#include "Log.h"
#include <stdio.h>

// this will be implemented in the client using our library
extern ChoreoEngine::Application* ChoreoEngine::CreateApplication();

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    ChoreoEngine::Log::Init();
    CF_CORE_WARN("Initiliazed Log!");
    int a{6};
    CF_INFO("Testing things Var={0}", a);
    auto app = ChoreoEngine::CreateApplication();

    app->run();
    delete app;
    return 0;
}
