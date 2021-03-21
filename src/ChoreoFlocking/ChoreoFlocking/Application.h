#pragma once

#include "Core.h"

namespace ChoreoFlocking{
    class Application {
    public:
        Application();
        virtual ~Application();
        void run();
    };

    // To be defined in CLIENT 
    Application* CreateApplication();
}
