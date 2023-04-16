#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include "../platform/platform_wrapper.hpp"
#include "../third_party/glms.hpp"

namespace as
{
    struct Transform
    {
        glm::vec3 position_{};
        glm::vec3 rotation_{};
        glm::vec3 scale_{};

        AS_SCRIPT_MEM_FUNC glm::vec3 right();
        AS_SCRIPT_MEM_FUNC glm::vec3 front();
        AS_SCRIPT_MEM_FUNC glm::vec3 up();
        AS_SCRIPT_MEM_FUNC glm::mat4 matrix();
        AS_SCRIPT_MEM_FUNC void set_relative_to(const Transform* trans);
    };

}; // namespace as

#endif // TRANSFORM_HPP
