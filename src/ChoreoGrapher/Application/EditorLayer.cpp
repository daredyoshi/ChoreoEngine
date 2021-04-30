
#include "EditorLayer.h"
#include "Application/Input.h"
#include "Application/Scene/Components.h"
#include "ChoreoApp.h"
#include "glm/gtc/type_ptr.hpp"


namespace ChoreoGrapher{

void EditorLayer::onAttach() {
    CE_PROFILE_FUNCTION();
    // temporary until a preferences menu is created
    // apologies I have a high-dpi display... just change the size if you don't
    // std::string fontFile = (ChoreoApp::Application::get().getRootDir() + "assets/fonts/Roboto_Slab/static/RobotoSlab-Regular.ttf");
    std::string fontFile = (ChoreoApp::Application::get().getRootDir() + "assets/fonts/droid-sans/DroidSans.ttf");
    // init ImGui things
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    m_font = io.Fonts->AddFontFromFileTTF(fontFile.c_str(), 23);

    // temp also 
    m_framebufferSpec.width = 1280;
    m_framebufferSpec.height= 720;

    m_framebuffer = ChoreoApp::Framebuffer::create(m_framebufferSpec);
    // m_framebuffer->invalidate();


    // Entity
    m_squareEntity = m_scene->createEntity("Square");
    m_squareEntity.addComponent<ChoreoApp::SpriteRendererComponent>(glm::vec4{0.8f, 0.2f, 0.2f, 1.0f}) ;

    // primary camera
    m_cameraEntity = m_scene->createEntity("Camera Entity");
    ChoreoApp::TransformComponent& xform = m_cameraEntity.getComponent<ChoreoApp::TransformComponent>();
    m_cameraEntity.addComponent<ChoreoApp::CameraComponent>();

    // m_secondCamera = m_scene->createEntity("Second Camera Entity");
    // auto& cc = m_secondCamera.addComponent<ChoreoApp::CameraComponent>();
    // cc.primary = false;



    // for scripting
    class CameraController : public ChoreoApp::ScriptableEntity {
    public:
        void onCreate(){
        }
        void onDestroy(){

        }
        void onUpdate(ChoreoApp::Timestep ts){
            // this assumes everything has  transform
            auto& transform = getComponent<ChoreoApp::TransformComponent>().transform;
            float speed = 0.1f * ts; 
            if (ChoreoApp::Input::isKeyPressed(CE_KEY_A))
                transform[3][0] -= speed;
            if (ChoreoApp::Input::isKeyPressed(CE_KEY_D))
                transform[3][0] += speed;
            if (ChoreoApp::Input::isKeyPressed(CE_KEY_W))
                transform[3][1] += speed;
            if (ChoreoApp::Input::isKeyPressed(CE_KEY_S))
                transform[3][1] -= speed;
            // std::cout << "Timestep: " << ts << std::endl;
        }
    };
    m_cameraEntity.addComponent<ChoreoApp::NativeScriptComponent>().bind<CameraController>();


}
void EditorLayer::onDetach() {
    CE_PROFILE_FUNCTION();
}


void EditorLayer::onUpdate(ChoreoApp::Timestep& timestep) {
    CE_PROFILE_FUNCTION();
    ChoreoApp::Renderer2D::resetStats();

    ChoreoApp::FramebufferSpecification spec = m_framebuffer->getSpecification();

    if((uint32_t)m_viewportSize.x > 0  && (uint32_t)m_viewportSize.y > 0 
            &&
            (spec.width != m_viewportSize.x || spec.height != m_viewportSize.y)){
        m_framebuffer->resize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
        m_camController.resize(m_viewportSize.x, m_viewportSize.y);
        m_scene->onViewportResize((uint32_t)m_viewportSize.x, (uint32_t)m_viewportSize.y);
    }

    if(m_viewportFocused && m_viewportHovered){
        CE_PROFILE_SCOPE("CamController::onUpdate");
        m_camController.onUpdate(timestep);

    }

    {
        CE_PROFILE_SCOPE("Draw Scene");
        m_framebuffer->bind();
        ChoreoApp::RenderCommand::setClearColor(glm::vec4{ 0.1f, 0.1f, 0.1f, 1.0f });
        ChoreoApp::RenderCommand::clear();

        m_scene->onUpdate(timestep);

        m_framebuffer->unbind();

    }
    if (ChoreoApp::Input::isMouseButtonPressed(CE_MOUSE_BUTTON_LEFT))
	{
	}
}



void EditorLayer::onEvent(ChoreoApp::Event& e) 
{
    m_camController.onEvent(e);
    if (e.getEventType() == ChoreoApp::EventType::WindowResize){
            auto& re = (ChoreoApp::WindowResizeEvent&) e;
    }
}

void EditorLayer::onImGuiRender() 
{
    CE_PROFILE_FUNCTION();
    ImGui::PushFont(m_font);
    // ImGui::ShowDemoWindow();


    // In 99% case you should be able to just call DockSpaceOverViewport() and ignore all the code below!

    // In this specific demo, we are not using DockSpaceOverViewport() because:
    // - we allow the host window to be floating/moveable instead of filling the viewport (when opt_fullscreen == false)
    // - we allow the host window to have padding (when opt_padding == true)
    // - we have a local menu bar in the host window (vs. you could use BeginMainMenuBar() + DockSpaceOverViewport() in your code!)
    // TL;DR; this demo is more complicated than what you would normally use.
    // If we removed all the options we are showcasing, this demo would become:
    //     void ShowExampleAppDockSpace()
    //     {
    //         ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
    //     }

    static bool dockspaceOpen = true;
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace", &dockspaceOpen, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File")){
            if (ImGui::MenuItem("Quit")) ChoreoApp::Application::get().close();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Options"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
            ImGui::MenuItem("Padding", NULL, &opt_padding);
            ImGui::Separator();

            if (ImGui::MenuItem("Close", NULL, false))
                dockspaceOpen= false;
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }


    // panels
    m_sceneHeirarchyPanel.onImGuiRender();
    m_entityPropertiesPanel.onImGuiRender();
    // viewoprt
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0.0f,0.0f});
    ImGui::Begin("ChoreoGrapher::Viewport");

    m_viewportFocused = ImGui::IsWindowFocused();
    m_viewportHovered = ImGui::IsWindowHovered();
    ChoreoApp::Application::get().getImGuiLayer()->setConsumeImGuiEvents(!m_viewportFocused || !m_viewportHovered);

    uint32_t textureId = m_framebuffer->getColorAttachmenRendererID(); 
    ImVec2 imViewportSize= ImGui::GetContentRegionAvail();
    glm::vec2 viewportSize{imViewportSize.x, imViewportSize.y};

    if(m_viewportSize != viewportSize && imViewportSize.x > 0 && imViewportSize.y > 0){
        m_viewportSize = viewportSize;
    }

    ImGui::Image((void*)(uintptr_t)(textureId), imViewportSize, ImVec2{0.0f, 1.0f}, ImVec2{1.0f, 0.0f});
    ImGui::End();
    ImGui::PopStyleVar(ImGuiStyleVar_WindowPadding);
    
    ImGui::End();
    ImGui::PopFont();
    // ImGui::ShowDemoWindow();
}
}
