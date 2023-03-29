#include "at_camera.hpp"

namespace ats
{

    Camera::Camera()
        : uniform_buffer_(VMA_MEMORY_USAGE_AUTO_PREFER_HOST, //
                          VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT)
    {
    }

    void Camera::create(Device device)
    {
        uniform_map_ = uniform_buffer_.create_mapped(device, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, //
                                                     VK_SHARING_MODE_EXCLUSIVE,                  //
                                                     sizeof(matrices_));

        buffer_info_.buffer = uniform_buffer_;
        buffer_info_.offset = 0;
        buffer_info_.range = VK_WHOLE_SIZE;
    }

    void Camera::destroy(Device device)
    {
        uniform_map_ = nullptr;
        uniform_buffer_.destroy(device);
    }

    void Camera::update(VkExtent2D viewport_extent)
    {
        front_.x = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
        front_.y = sin(glm::radians(pitch_));
        front_.z = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));

        front_ = glm::normalize(front_);
        up_ = glm::normalize(up_);

        matrices_.view_ = glm::lookAt(position_, position_ + front_, up_);
        matrices_.proj_ = glm::perspective(glm::radians(fov_),                                    //
                                           (float)viewport_extent.width / viewport_extent.height, //
                                           near_, far_);
        matrices_.proj_[1][1] *= -1;
        memcpy(uniform_map_, &matrices_, sizeof(matrices_));
    }

    glm::vec3 Camera::right() const
    {
        return glm::normalize(glm::cross(front_, up_));
    }

}; // namespace ats