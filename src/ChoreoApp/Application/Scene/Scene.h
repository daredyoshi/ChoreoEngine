#pragma once
#include "entt.hpp"
#include "Application/Timestep.h"
#include <memory>

// not too great here
namespace ChoreoGrapher{
    class SceneHierarchyPanel;
}

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




    private:
        friend class ChoreoGrapher::SceneHierarchyPanel;
        friend class Entity;
        entt::registry m_registry;
        uint32_t m_viewportWidth{0};
        uint32_t m_viewportHeight{0};
    };
}
