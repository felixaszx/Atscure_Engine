#ifndef AT_TRANSFORM_HPP
#define AT_TRANSFORM_HPP

#include "at_buffer.hpp"
#include "glms.hpp"

namespace ats
{
    class Transform : protected MultiType<glm::mat4>
    {
      protected:
        void update();

      public:
        glm::vec3 position_{};
        glm::vec3 rotation_{}; // in degree
        glm::vec3 scale_{1.0f, 1.0f, 1.0f};

        Transform();
    };

}; // namespace ats

#endif // AT_TRANSFORM_HPP
