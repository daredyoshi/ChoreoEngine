#include "cepch.h"
#include "Window.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace ChoreoEngine {

    class LinuxWindow : public Window {
    public:
        LinuxWindow(const WindowProps& props);
        virtual ~LinuxWindow();

        void onUpdate() override;

        inline unsigned int getWidth() const override { return m_data.width; }
        inline unsigned int getHeight() const override { return m_data.height; }

        inline void setEventCallback(const EventCallbackFn& callback) override { 
            m_data.eventCallback = callback; 
        }
        void setVSync(bool enabled) override;
        bool isVSync() const override;

        inline virtual void* getNativeWindow() const override { return m_window; };

    private:
        virtual void init(const WindowProps& props);
        virtual void shutdown();

        GLFWwindow* m_window;

        struct WindowData{
            std::string title;
            unsigned int width;
            unsigned int height;
            bool vSync;

            EventCallbackFn eventCallback;
        };

        WindowData m_data;
    };
}