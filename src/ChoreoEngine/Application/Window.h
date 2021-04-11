#pragma once

#include "cepch.h"

#include "Application/Core.h"
#include "Application/Events/Event.h"

namespace ChoreoEngine {
    struct WindowProps{
        std::string title;
        unsigned int width;
        unsigned int height;

        WindowProps(
                const std::string& Title = "ChoreoEngine",
                unsigned int Width = 1280,
                unsigned int Height = 720
                )
            : title(Title), width(Width), height(Height) {}
    };


    // Interface representing a desktop system based window
    // Everything else needs to be implemented per-platform
    class Window{
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        virtual ~Window() {}

        virtual void onUpdate() = 0;

        virtual unsigned int getWidth() const = 0;
        virtual unsigned int getHeight() const = 0;
        virtual float getTime() const = 0; 

        // Window attributes
        virtual void setEventCallback(const EventCallbackFn& callback) = 0;
        virtual void setVSync(bool enabled) = 0;
        virtual bool isVSync() const = 0;

        virtual void* getNativeWindow() const = 0;

        static Window* create(const WindowProps& props = WindowProps());
    };
}
