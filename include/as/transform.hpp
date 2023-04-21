#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "../third_party/glms.hpp"

constexpr glm::vec3 X_AXIS = {1.0f, 0.0f, 0.0f};
constexpr glm::vec3 Y_AXIS = {0.0f, 1.0f, 0.0f};
constexpr glm::vec3 Z_AXIS = {0.0f, 0.0f, 1.0f};

namespace as
{
    struct Transform
    {
        glm::vec3 position_ = {0.0f, 0.0f, 0.0f};
        glm::vec3 rotation_ = {0.0f, 0.0f, 0.0f}; // in degree
        glm::vec3 scale_ = {1.0f, 1.0f, 1.0f};
        glm::vec3 front_ = {1.0f, 0.0f, 0.0f};

        glm::vec3 right() const;
        glm::vec3 up() const;
        glm::mat4 matrix() const;
        void set_relative_to(const Transform* trans);
    };
}; // namespace as

#endif // TRANSFORM_HPP
