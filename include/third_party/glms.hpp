#ifndef GLMS_HPP
#define GLMS_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_INLINE
#define GLM_FORCE_XYZW_ONLY

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/quaternion.hpp>

namespace glms
{
    using namespace glm;

    template <typename T>
    GLM_FUNC_QUALIFIER mat<4, 4, T, defaultp> perspective(T fovy, T aspect, T zNear, T zFar)
    {
        mat<4, 4, T, defaultp> tmp = glm::perspective(fovy, aspect, zNear, zFar);
        tmp[1][1] *= -1;
        return tmp;
    }

    struct vec3 : public glm::vec3
    {
        vec3& operator=(const float* arr)
        {
            x = arr[0];
            y = arr[1];
            z = arr[2];

            return *this;
        }

        operator float*() { return &(*(this)[0]); }
        operator float*() const { return &(*(this)[0]); }
    };

    struct vec4 : public glm::vec4
    {
        vec4& operator=(const float* arr)
        {
            x = arr[0];
            y = arr[1];
            z = arr[2];
            z = arr[3];

            return *this;
        }

        operator float*() { return &(*(this)[0]); }
        operator float*() const { return &(*(this)[0]); }
    };

    struct quat : public glm::quat
    {
        quat& operator=(const float* arr)
        {
            x = arr[0];
            y = arr[1];
            z = arr[2];
            z = arr[3];

            return *this;
        }

        operator float*() { return &(*(this)[0]); }
        operator float*() const { return &(*(this)[0]); }
    };

} // namespace glms

#endif // GLMS_HPP
