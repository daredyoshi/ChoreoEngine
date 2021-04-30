#pragma once

#include "glm/geometric.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/euler_angles.hpp"
#include <string>
#include "SceneCamera.h"
#include "ScriptableEntity.h"

namespace ChoreoApp{
    struct TagComponent{
        std::string tag;

        TagComponent() = default;
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string&  _tag)
            : tag(_tag) {}

        operator std::string&() { return tag; }
    };

    struct TransformComponent{
        glm::mat4 transform = glm::mat4{1.0f};

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::mat4& _transform)
            : transform(_transform) {}

        // glm::vec3 getTranslation() const {
        //     return glm::vec3{transform[3][0], transform[3][1], transform[3][2]};
        // }
        //
        // glm::vec3 getEulerRotation() const {
        //     float x, y, z;
        //     glm::extractEulerAngleXYZ(transform, x, y, z);
        //     return glm::vec3{x, y, z};
        // }
        //
        // glm::vec3 getScale() const {
        //     return glm::vec3{
        //         glm::length( glm::vec3{transform[0][0], transform[0][1], transform[0][2]} ),
        //         glm::length( glm::vec3{transform[1][0], transform[1][1], transform[1][2]} ),
        //         glm::length( glm::vec3{transform[2][0], transform[2][1], transform[2][2]} )
        //     };
        // }

        operator glm::mat4() { return transform; }
    };

    struct CameraComponent{
        SceneCamera camera;
        bool fixedAspectRatio{false};
        bool primary{true}; // TODO think about moving to scene

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
    };

    struct NativeScriptComponent{
        ScriptableEntity* instance = nullptr;


        ScriptableEntity*(*instantiateScript)();
        void(*destoryScript)(NativeScriptComponent*);

        template<typename T>
        void bind(){
            instantiateScript = []() {return static_cast<ScriptableEntity*> ( new T() ); };
            destoryScript = [](NativeScriptComponent* nsc) { delete nsc->instance; nsc->instance = nullptr;};
        }
    };

    struct SpriteRendererComponent{
        glm::vec4 color = glm::vec4{1.0f, 1.0f, 1.0f, 1.0f};

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const glm::vec4& _color)
            : color(_color) {}
    };


}
