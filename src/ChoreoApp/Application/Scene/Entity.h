#pragma once


#include "Application/Log.h"
#include "Scene.h"
#include "entt.hpp"
#include <memory>
#include <utility>

namespace ChoreoApp {
    class Entity {

    public:
        Entity() = default;
        Entity(entt::entity handle, std::weak_ptr<Scene> scene);
        Entity(const Entity& other) = default;

        entt::entity getEntityHandle() const { return m_entityHandle; }
        std::weak_ptr<Scene> getScene() const { return m_scene; }

        template<typename T>
        bool hasComponent(){
            return m_scene.lock()->m_registry.any_of<T>(m_entityHandle);
        }

        template<typename T>
        T& getComponent() const{
            CE_CORE_ASSERT(getComponent<T>(), "Entity does not have component!");
            return m_scene.lock()->m_registry.get<T>(m_entityHandle);
        }

        template<typename T, typename... Args>
        T& addComponent(Args&&... args){
            CE_CORE_ASSERT(!hasComponent<T>(), "Entity already has comopnent!");
            return m_scene.lock()->m_registry.emplace<T>(m_entityHandle, std::forward<Args>(args)...);
        }

        template<typename T>
        void removeComponent(){
        CE_CORE_ASSERT(getComponent<T>(), "Entity does not have component!");
            m_scene.lock()->m_registry.remove<T>(m_entityHandle);
        }

        operator bool() const { return m_entityHandle  != entt::null; }
     
    private:
        entt::entity m_entityHandle{entt::null};
        std::weak_ptr<Scene> m_scene;
    };
}
