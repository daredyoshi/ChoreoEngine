
#include "EntityPropertiesPanel.h"
#include "Application/Core.h"
#include "Application/Scene/Controller.h"
#include "ChoreoApp.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/trigonometric.hpp"
#include "imgui.h"
#include "Ui/Widgets/ControllerEdit.h"
#include <memory>

namespace ChoreoGrapher{


    EntityPropertiesPanel::EntityPropertiesPanel(const ChoreoApp::Ref<ChoreoApp::Scene>& scene){
        setContext(scene); 
    }

    void EntityPropertiesPanel::setContext(const ChoreoApp::Ref<ChoreoApp::Scene>& scene){
        m_context = scene;;
    }

    void EntityPropertiesPanel::onImGuiRender(){
        ImGui::Begin("ChoreoGrapher::Properties");
        auto lastSelectedEntity = m_context->getLastSelectedEntity();
        auto test2 = lastSelectedEntity->getEntityHandle();
        auto test = *lastSelectedEntity;
        if(*lastSelectedEntity)
            drawEntityComponents(lastSelectedEntity); 
        ImGui::End();
    }

    void EntityPropertiesPanel::drawEntityComponents(ChoreoApp::Ref<ChoreoApp::Entity> entity){
        if(entity->hasComponent<ChoreoApp::TagComponent>()){
            auto& tag = entity->getComponent<ChoreoApp::TagComponent>().tag ;

            // if we go over this everythig will die
            char buffer[256];
            // initialize to 0
            memset(buffer, 0, sizeof(buffer));
            strcpy(buffer, tag.c_str());
            if (ImGui::InputText("Tag", buffer, sizeof(buffer))){
                tag = std::string(buffer);
            }
        }

        const ChoreoApp::Scope<ChoreoApp::Time>& t {m_context->getTime()};

        // Xform  
        if(entity->hasComponent<ChoreoApp::XformComponent>()){
            if (ImGui::TreeNodeEx((void*)typeid(ChoreoApp::XformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform") ){
                auto& XformComponent = entity->getComponent<ChoreoApp::XformComponent>();
                ChoreoApp::Ref<ChoreoApp::XformController> xformController = XformComponent.xform; 
              
                // if this is an Euler Xform Controller
                if(xformController->getType() == ChoreoApp::XformController::XformType::Euler){
                    glm::vec3 p { xformController->evalPosition(t) };
                    if (ImGui::DragFloat3("Position", glm::value_ptr(p), 0.1f)){
                        xformController->setPositionAtTime(t, p);
                    }        
                    glm::vec3 r { xformController->evalEulerAngles(t) };
                    if(ImGui::DragFloat3("Euler Angles", glm::value_ptr(r), 0.5f)){
                        xformController->setEulerAnglesAtTime(t, r);
                    }
                    glm::vec3 s { xformController->evalScale(t) };
                    if(ImGui::DragFloat3("Scale", glm::value_ptr(s), 0.01f)){
                        xformController->setScaleAtTime(t, s);
                    }
                }


                ImGui::TreePop();
            }
        }

        // SPRITE RENDERER
        if(entity->hasComponent<ChoreoApp::SpriteRendererComponent>()){
            if (ImGui::TreeNodeEx((void*)typeid(ChoreoApp::SpriteRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "SpriteRendererComponent") ){
                auto& color = entity->getComponent<ChoreoApp::SpriteRendererComponent>().color;
                ImGui::ColorEdit4("Color", glm::value_ptr(color));

                ImGui::TreePop();
            }
        }
        

        // CAMERA
        if(entity->hasComponent<ChoreoApp::CameraComponent>()){
            if (ImGui::TreeNodeEx((void*)typeid(ChoreoApp::CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera") ){
                auto& cameraComponent= entity->getComponent<ChoreoApp::CameraComponent>();
                auto& camera = cameraComponent.camera;
                const char* projectiontypeStrings[] = { "Perspective", "Orthographic" };
                const char* currentProjectionTypeString = projectiontypeStrings[(int)cameraComponent.camera.getProjectionType()];
                if (ImGui::BeginCombo("ProjectionType", currentProjectionTypeString)){
                    for (int i{0}; i<2; ++i){
                        // if this is the selected one
                        bool isSelected = currentProjectionTypeString == projectiontypeStrings[i];
                        // if clicked on it
                        if (ImGui::Selectable(projectiontypeStrings[i], isSelected)){
                            currentProjectionTypeString = projectiontypeStrings[i]; 
                            cameraComponent.camera.setProjectionType((ChoreoApp::SceneCamera::ProjectionType)i);
                        }
                        if (isSelected)
                            ImGui::SetItemDefaultFocus();
                    } 
                    ImGui::EndCombo();
                }

                ImGui::Checkbox("Primary", &cameraComponent.primary);

                // PERSPECTIVE 
                if (camera.getProjectionType() == ChoreoApp::SceneCamera::ProjectionType::Perspective){
                    // float perspFOV = glm::degrees(camera.getPerspectiveVerticalFOV());
                    // ImGui::InputDouble("Vertical FOV", 
                    // float perspectiveFOV{ glm::degrees(camera.getPerspectiveFOV()->eval(t))};
                    if(camera.getPerspectiveFOV()->getType() == ChoreoApp::Controller<float>::ControllerType::Animated){
                        ChoreoApp::Ref<ChoreoApp::FloatController> controller = camera.getPerspectiveFOV();
                        if(FloatControllerEdit("Vertical FOV", controller, t)){
                            // CE_TRACE("edited"); 
                        }
                    }
                    // if (ImGui::DragFloat("Vertical FOV", &perspectiveFOV)){
                    //     camera.setPerspectiveFOV(glm::radians(perspectiveFOV), m_context->getTime());
                    // }
                    float perspectiveNearClip{ camera.getPerspectiveNearClip()->eval(t)};
                    if (ImGui::DragFloat("Vertical NearClip", &perspectiveNearClip)){
                        camera.setPerspectiveNearClip(perspectiveNearClip, m_context->getTime());
                    }
                    float perspectiveFarClip{ camera.getPerspectiveFarClip()->eval(t)};
                    if (ImGui::DragFloat("Vertical FarClip", &perspectiveFarClip)){
                        camera.setPerspectiveFarClip(perspectiveFarClip, m_context->getTime());
                    }
                }

                // ORTHOGARPHIC
                if (camera.getProjectionType() == ChoreoApp::SceneCamera::ProjectionType::Orthographic){

                    // float orthoSize = camera.getOrthographicSize();
                    // if (ImGui::DragFloat("Size", &orthoSize))
                    //     camera.setOrthographicSize(orthoSize);
                    //
                    // float orthoNear = camera.getOrthographicNearClip();
                    // if (ImGui::DragFloat("Near", &orthoNear))
                    //     camera.setOrthographicNearClip(orthoNear);
                    //
                    // float orthoFar = camera.getOrthographicFarClip();
                    // if (ImGui::DragFloat("Far", &orthoFar))
                    //     camera.setOrthographicFarClip(orthoFar);
                    //
                    // ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.fixedAspectRatio);
                }

                ImGui::TreePop();
            }
        }
         
    }
}
