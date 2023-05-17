#ifndef DEFFERED_ZERO_HPP
#define DEFFERED_ZERO_HPP

#include <semaphore>
#include <thread>
#include <atomic>
#include "programs.hpp"

namespace as
{
    struct DefferedZero : public RenderProgramBase
    {
        struct UniformBuffer
        {
            glm::mat4 view_{1.0f};
            glm::mat4 proj_{1.0f};
        } ubo;
        vk::DescriptorBufferInfo ubo_info_{};
        vk::WriteDescriptorSet ubo_write_{};
        UniqueObj<Buffer> uniform_buffer_{nullptr};

        DefferedZero(VirtualObj<Swapchain> swapchian);
        ~DefferedZero();

        void update_ubo(glm::mat4 view, glm::mat4 proj);
    };
}; // namespace as

#endif // DEFFERED_ZERO_HPP
