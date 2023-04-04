#ifndef AT_TRANSFORM_HPP
#define AT_TRANSFORM_HPP

#include "as_buffer.hpp"
#include "glms.hpp"

namespace as
{
    struct Transform : iMultiType(glm::mat4)
    {
        glm::vec3 position_{};
        glm::vec3 rotation_{};
        glm::vec3 scale_{1.0f, 1.0f, 1.0f};

        Transform();
        void move(glm::vec3 direction, float distance);
        void update();
    };

}; // namespace as

#endif // AT_TRANSFORM_HPP
