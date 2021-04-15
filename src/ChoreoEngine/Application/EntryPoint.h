#pragma once

#include "Application/Application.h"
#include "Application/Log.h"
#include <stdio.h>

// this will be implemented in the client using our library
extern ChoreoEngine::Application* ChoreoEngine::CreateApplication(const std::string& rootDir);

int main(int argc, char** argv)
{
    (void)argc;
        // Get the last position of '/'
    std::string aux(argv[0]);
    // we aren't using numArgs

    // get '/' or '\\' depending on unix/mac or windows.
#if defined(_WIN32) || defined(WIN32)
    int pos = aux.rfind('\\');
#else
    int pos = aux.rfind('/');
#endif

    // Get the path and the name
    const std::string rootDir= aux.substr(0,pos+1);

    ChoreoEngine::Log::Init();
    CE_CORE_WARN("Initiliazed Log!");
    auto app = ChoreoEngine::CreateApplication(rootDir);

    app->run();
    delete app;
    return 0;
}
