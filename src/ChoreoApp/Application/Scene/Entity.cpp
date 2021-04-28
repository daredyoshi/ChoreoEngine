
#include "capch.h"
#include "Entity.h"

namespace ChoreoApp {
    Entity::Entity(entt::entity handle, std::weak_ptr<Scene> scene)
        : m_entityHandle{handle}, m_scene{scene}
    {
        
    }


}
