#pragma once
#include "entt.hpp"
#include "Application/Timestep.h"
#include <memory>


namespace ChoreoApp {
    class Entity;

    class Scene : public std::enable_shared_from_this<Scene>{
    public:
        Scene();
        ~Scene();

        void onUpdate(Timestep ts);
        // refactor this to Entity createEntity()
        Entity createEntity(const std::string& name);



    private:
        friend class Entity;
        entt::registry m_registry;
    };
}
