#pragma once

#include "glm/glm.hpp"
#include <string>
#include "Application/Renderer/Camera.h"

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

        operator glm::mat4() { return transform; }
    };

    struct CameraComponent{
        Camera camera{glm::mat4{1.0}};
        bool primary{true}; // TODO think about moving to scene

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
        CameraComponent(const glm::mat4& projectionMatrix)
            : camera(projectionMatrix) {}
    };

    struct SpriteRendererComponent{
        glm::vec4 color = glm::vec4{1.0f, 1.0f, 1.0f, 1.0f};

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const glm::vec4& _color)
            : color(_color) {}
    };


}
