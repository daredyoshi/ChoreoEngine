#pragma once


#include "Entity.h"
#include "Application/Timestep.h"

namespace ChoreoApp {
    class ScriptableEntity{
    public:
        virtual ~ScriptableEntity() {}

        // just a wrapper
        template<typename T>
        T& getComponent(){
            return m_entity.getComponent<T>();
        }

    protected:
        virtual void onCreate(){};
        virtual void onDestroy(){};
        virtual void onUpdate(Timestep ts, Scene& scene){(void(ts)); (void(scene));};
    private:
        Entity m_entity;
        friend class Scene;
    };
}
