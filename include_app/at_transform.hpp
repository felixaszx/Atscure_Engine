#ifndef AT_TRANSFORM_HPP
#define AT_TRANSFORM_HPP

#include "at_buffer.hpp"
#include "glms.hpp"

namespace ats
{
    struct Transform : protected MultiType<glm::mat4>
    {
        glm::vec3 position_{};
        glm::vec3 rotation_{};
        glm::vec3 scale_{1.0f, 1.0f, 1.0f};

        Transform();
        void update();
    };

}; // namespace ats

#endif // AT_TRANSFORM_HPP
