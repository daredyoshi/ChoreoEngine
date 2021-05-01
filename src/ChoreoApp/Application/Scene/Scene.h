#pragma once
#include "entt.hpp"
#include "Application/Core.h"
#include "Application/Timestep.h"
#include "Application/Scene/Time.h"
#include <memory>

// inherit from ScenePanel to be friend to Scene

namespace ChoreoApp {

    class Entity;

    class Scene : public std::enable_shared_from_this<Scene>{
    public:
        Scene();
        ~Scene();

        void onUpdate(Timestep ts);
        void onViewportResize(const uint32_t width, const uint32_t height);
        // refactor this to Entity createEntity()
        Entity createEntity(const std::string& name);

        entt::registry& getRegistry() { return m_registry; }        

        const Scope<Time>& getTime() { return m_time; }
        // TODO: update all scene component controllers when time is set
        void setTimeTick(const uint32_t tick) { m_time->setTick(tick); }

        // void setLastSelectedEntity(const uint32_t entityHandle) { m_lastSelectedEntityHandle = entityHandle; };
        void setLastSelectedEntity(Ref<Entity> entity) { m_lastSelectedEntity = entity; };
        void clearLastSelectedEntity() { m_lastSelectedEntity = CreateRef<Entity>(); }
        Ref<Entity> getLastSelectedEntity() { return m_lastSelectedEntity; };



    private:
        entt::registry m_registry;
        Scope<Time> m_time{CreateScope<Time>()};


        Ref<Entity> m_lastSelectedEntity{CreateRef<Entity>()};

        uint32_t m_viewportWidth{0};
        uint32_t m_viewportHeight{0};
    };
}