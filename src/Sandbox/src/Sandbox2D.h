# pragma once
#include "Application/Timestep.h"
#include "ChoreoEngine.h"

class Sandbox2D : public ChoreoEngine::Layer{
public:
    Sandbox2D()
        : 
         Layer("Sandbox2D"),
         m_camController{1280.0f / 780.0f, true},
         m_squarePos{0}
         {};
    virtual ~Sandbox2D() {};
    void onUpdate(ChoreoEngine::TimeStep& timestep) override;
    void onImGuiRender() override;
    void onEvent(ChoreoEngine::Event& e) override;

    void onAttach() override;
    void onDetach() override;

private:

    ChoreoEngine::ShaderLibrary m_shaderLibrary;

    ChoreoEngine::Ref<ChoreoEngine::VertexArray> m_SquareVA;

    ChoreoEngine::OrthographicCameraController m_camController;

    glm::vec3 m_squarePos{0};
    glm::vec3 m_squareCol{0.8, 0.2, 0.2};
};