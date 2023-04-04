#ifndef AT_CAMERA_HPP
#define AT_CAMERA_HPP

#include "as_device.hpp"
#include "as_buffer.hpp"
#include "glms.hpp"

namespace as
{
    class Camera
    {
      private:
        Buffer uniform_buffer_;
        void* uniform_map_ = nullptr;

      public:
        VkDescriptorBufferInfo buffer_info_{};

        struct CameraUniformBuffer
        {
            glm::mat4 view_{};
            glm::mat4 proj_{};
        };
        CameraUniformBuffer matrices_{};

        glm::vec3 position_ = {0.0f, 0.0f, 0.0f};
        glm::vec3 up_ = {0.0f, 1.0f, 0.0f};
        glm::vec3 front_{};

        float yaw_ = 0.0f;
        float pitch_ = 0.0f;
        float fov_ = 45.0f;

        float near_ = 0.5f;
        float far_ = 1000.0f;

        Camera();

        void create(Device device);
        void destroy(Device device);

        void update(VkExtent2D viewport_extent);
        glm::vec3 right() const;
    };
}; // namespace as

#endif // AT_CAMERA_HPP
