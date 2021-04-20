#include "Sandbox2D.h"
#include "ChoreoEngine.h"
#include "glm/ext/matrix_transform.hpp"
#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"
#include <chrono>

template<typename Fn>
class Timer{
public:
    Timer(const char* name, Fn&& func)
        : m_name{name}, m_stopped{false}, m_func{func}{
            m_startTimepoint = std::chrono::high_resolution_clock::now();
            (void)m_name;
        }
    ~Timer(){
        if(!m_stopped)
            stop();
    }

    void stop(){
        auto endTimepoint = std::chrono::high_resolution_clock::now();

        long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_startTimepoint).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

        m_stopped = true;

        float duration = (end - start) * 0.001f;

        // std::cout << "Ductation: " << m_name << " " << duration << "ms" <<  std::endl;
        m_func({m_name, duration});
    }

    const char* getName() const { return m_name; } 


private:
    const char* m_name;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_startTimepoint;
    bool m_stopped;
    Fn m_func;
};


void Sandbox2D::onAttach() {
}
void Sandbox2D::onDetach() {

}

#define TOKEN_PASTE(x, y) x##y
#define CAT(x,y) TOKEN_PASTE(x,y)
#define PROFILE_SCOPE(name) Timer CAT(timer, __LINE__)(name, [&](ProfileResult profileResult) { m_profileResults.push_back(profileResult); } )


void Sandbox2D::onUpdate(ChoreoEngine::TimeStep& timestep) {
    PROFILE_SCOPE("Sandbox2D::onUpdate");
    {
        PROFILE_SCOPE("Sandbox2D::camController::onUpdate");
        m_camController.onUpdate(timestep);

    }
    ChoreoEngine::RenderCommand::setClearColor(glm::vec4{ 0.1f, 0.1f, 0.1f, 1.0f });
    ChoreoEngine::RenderCommand::clear();

    ChoreoEngine::Renderer2D::beginScene(m_camController.getCamera());
    ChoreoEngine::Renderer2D::drawQuad(m_squarePos, m_squareAngle, m_squareScale, m_squareCol);
    ChoreoEngine::Renderer2D::drawQuad(m_squarePos + glm::vec3{0.2, 0.2, 0.0}, m_squareAngle, m_squareScale, m_texture, m_squareCol);
    ChoreoEngine::Renderer2D::endScene();
}



void Sandbox2D::onEvent(ChoreoEngine::Event& e) 
{
    m_camController.onEvent(e);
    if (e.getEventType() == ChoreoEngine::EventType::WindowResize){
            auto& re = (ChoreoEngine::WindowResizeEvent&) e;
    }
}

void Sandbox2D::onImGuiRender() 
{
        ImGui::Begin("ChoreoEngine::Settings");
        ImGui::ColorEdit4("Square Color", glm::value_ptr(m_squareCol));
        ImGui::DragFloat3("Square Pos", glm::value_ptr(m_squarePos), 0.001f);
        ImGui::DragFloat2("Square Scale", glm::value_ptr(m_squareScale), 0.001f);
        ImGui::DragFloat("Square Angle", &m_squareAngle, 2.0);

        for( auto& result: m_profileResults ){
            char label[50];
            strcpy(label, " %.3fms :: ");
            strcat(label, result.name);
            ImGui::Text(label, result.time);
        }
        m_profileResults.clear();

        ImGui::End();
        // ImGui::ShowDemoWindow();
}

