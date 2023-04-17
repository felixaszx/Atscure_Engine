#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "../platform/platform_wrapper.hpp"
#include "../third_party/glms.hpp"

#define X_AXIS glm::vec3(1.0f, 0.0f, 0.0f)
#define Y_AXIS glm::vec3(0.0f, 1.0f, 0.0f)
#define Z_AXIS glm::vec3(0.0f, 0.0f, 1.0f)

namespace as
{
    struct Transform
    {
        glm::vec3 position_ = {0.0f, 0.0f, 0.0f};
        glm::vec3 rotation_ = {0.0f, 0.0f, 0.0f}; // in degree
        glm::vec3 scale_ = {1.0f, 1.0f, 1.0f};
        glm::vec3 front_ = {1.0f, 0.0f, 0.0f};

        AS_SCRIPT_MEM_FUNC glm::vec3 right();
        AS_SCRIPT_MEM_FUNC glm::vec3 up();
        AS_SCRIPT_MEM_FUNC glm::mat4 matrix();
        AS_SCRIPT_MEM_FUNC void set_relative_to(const Transform* trans);
    };

}; // namespace as

#endif // TRANSFORM_HPP
