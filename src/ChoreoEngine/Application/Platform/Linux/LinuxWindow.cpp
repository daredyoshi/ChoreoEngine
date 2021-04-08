#include "cepch.h"


#include "LinuxWindow.h"
#include "Events/Application.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"

namespace ChoreoEngine{
    static bool s_GLFWInitialized{ false };

    static void GLFWErrorCallback(int error, const char* description){
        CE_CORE_ERROR("GLFW Error ( {0} ): {1}", error, description);
    }

    // this overwrites the window constructor with a linux window constructor
    // will only get done if this file is added to sources
    // for other platforms add the other platforms to sources instead
    Window* Window::create(const WindowProps& props){
        return new LinuxWindow(props);
    }

    LinuxWindow::LinuxWindow(const WindowProps& props){
        init(props);
    }

    LinuxWindow::~LinuxWindow(){
        shutdown(); 
    }

    void LinuxWindow::init(const WindowProps& props){
        m_data.title = props.title;
        m_data.width = props.width;
        m_data.height = props.height;

        CE_CORE_INFO("Creating window {0} ({1}, {2})", 
                props.title, props.width, props.height); 

        if (!s_GLFWInitialized){
            int success = glfwInit();
            CE_CORE_ASSERT(success, "Could not initialize GLFW!");            

            glfwSetErrorCallback(GLFWErrorCallback);

            s_GLFWInitialized = true;
        } 

        // Set glfw to use open gl version 3.3 
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            
        m_window = glfwCreateWindow((int)props.width, (int)props.height, "LearnOpenGL", NULL, NULL);
        glfwMakeContextCurrent(m_window);

        if ( m_window == NULL ){
            CE_CORE_ERROR( "Failed to create GLFW window" );
            glfwTerminate();
        }
        // this was in the open gl tutorial BUT it makes linking problems with le sandbox app
        // initilize glad
        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
            CE_CORE_ERROR("Failed to initialize GLAD" );
        }

        // this passes data into the callbacks of the window
        glfwSetWindowUserPointer(m_window, &m_data);
        setVSync(true);

        // set GLFW callbacks
        glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height){
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            data.width = width;
            data.height = height;

            WindowResizeEvent event(width, height);
            data.eventCallback(event);
        });

        glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window){    
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            data.eventCallback(event);
        });

        glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods){         
            (void)mods;
            (void)scancode;
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            switch(action){
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(key, 0);
                    data.eventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(key);
                    data.eventCallback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent event(key, 1);
                    data.eventCallback(event);
                    break;
                }
            }
        });

        glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int keyCode){
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            KeyTypedEvent event(keyCode);
            data.eventCallback(event);
        });

        glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods){  
            (void)mods;
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            switch(action){
                case GLFW_PRESS:{
                    MouseButtonPressedEvent event(button);
                    data.eventCallback(event);
                    break;
                }
                case GLFW_RELEASE:{
                    MouseButtonReleasedEvent event(button);
                    data.eventCallback(event);
                    break;
                }
            }
        });

        glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset){ 
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            MouseScrollEvent event((float)xOffset, (float)yOffset);
            data.eventCallback(event);

        });

        glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos){
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            MouseMovedEvent event((float)xPos, (float)yPos);
            data.eventCallback(event);
        });

    }

    void LinuxWindow::shutdown(){
        glfwDestroyWindow(m_window);
    }

    void LinuxWindow::onUpdate(){
         
        glfwPollEvents();
        glfwSwapBuffers(m_window);
    }

    void LinuxWindow::setVSync(bool enabled){
        if(enabled){
            glfwSwapInterval(1);
        }
        else{
            glfwSwapInterval(0);
        }
        m_data.vSync = enabled;
    }

    bool LinuxWindow::isVSync() const{
        return m_data.vSync;
    }
}
