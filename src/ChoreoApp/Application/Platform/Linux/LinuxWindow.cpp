

#include "Application/Platform/Linux/LinuxWindow.h"
#include "Application/Platform/OpenGL/OpenGLContext.h"
#include "Application/Events/Application.h"
#include "Application/Events/MouseEvent.h"
#include "Application/Events/KeyEvent.h"
#include "Application/Renderer/GraphicsContext.h"



namespace ChoreoApp{
    static bool s_GLFWInitialized{ false };

    static void GLFWErrorCallback(int error, const char* description){
        CE_CORE_ERROR("GLFW Error ( {0} ): {1}", error, description);
        (void)error;
        (void)description;
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
        CE_PROFILE_FUNCTION();

        m_data.title = props.title;
        m_data.width = props.width;
        m_data.height = props.height;

        CE_CORE_INFO("Creating window {0} ({1}, {2})", 
                props.title, props.width, props.height); 


        if (!s_GLFWInitialized){
            CE_PROFILE_SCOPE("glfwInit");
            int success = glfwInit();
            CE_CORE_ASSERT(success, "Could not initialize GLFW!");            

            glfwSetErrorCallback(GLFWErrorCallback);

            s_GLFWInitialized = true;
        } 

            
        // set X11 flags
        glfwWindowHint(GLFW_FLOATING, GLFW_TRUE) ;
        glfwWindowHintString(GLFW_X11_CLASS_NAME, "ChoreoApp") ;
        {
            CE_PROFILE_SCOPE("glfwCreateWindow");
            m_window = glfwCreateWindow((int)props.width, (int)props.height, m_data.title.c_str(), NULL, NULL);
        }
        m_context = new OpenGLContext(m_window);
        m_context->Init();


        if ( m_window == NULL ){
            CE_CORE_ERROR( "Failed to create GLFW window" );
            glfwTerminate();
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

    float LinuxWindow::getTime() const { 
        return (float)glfwGetTime();
    }

    void LinuxWindow::shutdown(){
        glfwDestroyWindow(m_window);
    }

    void LinuxWindow::onUpdate(){
        CE_PROFILE_FUNCTION();
        // TODO: change this to glfwWaitEvents() but only when not viewport focused
        glfwPollEvents();
        m_context->SwapBuffers();
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
