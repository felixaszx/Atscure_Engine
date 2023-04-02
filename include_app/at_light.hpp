#ifndef AT_LIGHT_HPP
#define AT_LIGHT_HPP

#include "at_device.hpp"
#include "glms.hpp"

namespace ats
{
    struct DirectionalLight
    {
        glm::vec4 camera_;
        glm::vec4 color_;
        glm::vec3 direction_;
        glm::vec3 position_;
        float strength;
    };

    struct PointLight
    {
        glm::vec4 camera_;
        glm::vec4 color_;
        glm::vec3 direction_;
        glm::vec3 position_;
        float strength;

        float constant;
        float linear;
        float quadratic;
    };

}; // namespace ats

#endif // AT_LIGHT_HPP
