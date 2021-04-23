#pragma once

#include "Application/Application.h"
#include "Application/Debug/Instrumentor.h"
#include "Application/Log.h"
#include <stdio.h>

// this will be implemented in the client using our library
extern ChoreoApp::Application* ChoreoApp::CreateApplication(const std::string& rootDir);

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

    ChoreoApp::Log::Init();
    CE_PROFILE_BEGIN_SESSION("Startup", "ChoreoApp-Startup.json");
    auto app = ChoreoApp::CreateApplication(rootDir);
    CE_PROFILE_END_SESSION()
    CE_PROFILE_BEGIN_SESSION("Runtime", "ChoreoApp-Runtime.json");

    app->run();
    CE_PROFILE_END_SESSION()
    CE_PROFILE_BEGIN_SESSION("Shutdown", "ChoreoApp-Shutdown.json");
    delete app;
    CE_PROFILE_END_SESSION()
    return 0;
}
