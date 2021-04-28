#include "capch.h"
#include "Scene.h"
#include "glm/glm.hpp"
#include "Components.h"
#include "Application/Renderer/Renderer.h"
#include "Entity.h"
#include <memory>

namespace ChoreoApp {

    // EXAMPLE USE 

    // static void doMath(const glm::mat4& transform){
    //     (void)transform;
    // }
    //
    // // example of a component       
    // //
    // struct MeshComponent{
    //     // blah
    // };
    // struct TransformComponent{
    //     glm::mat4 transform;
    //
    //     TransformComponent() = default;
    //     TransformComponent(const TransformComponent&) = default;
    //     TransformComponent(const glm::mat4& transform)
    //         : transform(transform) {}
    //
    //     operator glm::mat4() { return transform; }
    // };
    // //
    // // TransformComponent transform;
    // //
    // // This is nice and readable
    // // doMath(transform);
    //
    //
    // // this is a uint pretty much
    // entt::entity entity  = m_registry.create();
    // auto& transform = m_registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));
    // m_registry.remove<TransformComponent>(entity);
    // m_registry.clear<TransformComponent>();
    // if(m_registry.has<TransformComponent>(entity))
    //     transform = m_registry.get<TransformComponent>(entity);
    //
    // //
    // auto view = m_registry.view<TransformComponent>();
    // for (auto view_entity: view){
    //     TransformComponent& view_transform = view.get<TransformComponent>(view_entity);
    //     doMath(view_transform);
    // }
    //
    // auto group = m_registry.group<TransformComponent>(entt::get<MeshComponent>);
    // for (auto group_entity : group){
    //     auto&[group_transform, group_mesh] = group.get<TransformComponent, MeshComponent>(group_entity);
    //     Renderer::Submit(mesh, transform);
    // }


    Scene::Scene(){
    }

    Scene::~Scene(){

    }

    Entity Scene::createEntity(const std::string& name = ""){
        Entity entity ={ m_registry.create(), shared_from_this()}; 
        // all entities should have a transform component
        entity.addComponent<TransformComponent>();
        // all entities should have a name
        auto& tagComponent { entity.addComponent<TagComponent>(name) };
        tagComponent.tag = name.empty() ? "Entity" : name;

        return entity;
    }

    void Scene::onUpdate(Timestep ts){
        (void)ts;

        // Update scripts
        { 
            m_registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc){
                (void)entity;
                // TODO: Move to Scene:OnSceneRun
                if (!nsc.instance){
                    nsc.instance = nsc.instantiateScript();
                    nsc.instance->m_entity = Entity{ entity, shared_from_this()};
                    nsc.instance->onCreate();
                }
                nsc.instance->onUpdate(ts);
            });
        }

        Camera* mainCamera = nullptr;
        glm::mat4 cameraTransform;
        {
            auto view = m_registry.view<TransformComponent, CameraComponent>();
            for (auto entity : view){
                auto [ transform, camera ] =  view.get<TransformComponent, CameraComponent>(entity);
                if(camera.primary){
                    mainCamera = &camera.camera; 
                    cameraTransform = transform.transform;
                    break;
                }
            }
        }

        if(mainCamera){

            ChoreoApp::Renderer2D::beginScene(*mainCamera, cameraTransform);
            auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
            for (auto entity: group){
                auto [transform , spriteRenderer] = group.get<TransformComponent, SpriteRendererComponent>(entity);
                Renderer2D::drawQuad(transform, spriteRenderer.color);
            }
            ChoreoApp::Renderer2D::endScene();
        }
    }

    void Scene::onViewportResize(const uint32_t width, const uint32_t height){
        m_viewportWidth = width;
        m_viewportHeight = height;

        // Resize our non fixed ratio cameras
        auto view = m_registry.view<CameraComponent>();
        for (auto entity : view){
            auto& cameraComponent =  view.get<CameraComponent>(entity);
            if (!cameraComponent.fixedAspectRatio){
                cameraComponent.camera.setViewportSize(width, height);
            }
        }
    }
}
