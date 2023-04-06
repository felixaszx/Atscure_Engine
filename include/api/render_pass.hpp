#ifndef RENDER_PASS_HPP
#define RENDER_PASS_HPP

#include "image.hpp"
#include "device.hpp"

namespace as
{

    struct RenderPass : vk::RenderPass, //
                        public DeviceRAII
    {
        struct Detail
        {
            std::vector<vk::SubpassDependency> dependency_;
            std::vector<vk::SubpassDescription> subpass_;
            std::vector<vk::AttachmentDescription> attachments_;
            std::vector<std::vector<vk::AttachmentReference>> reference_;
        };

        RenderPass(const RenderPass::Detail& details);
        ~RenderPass() override;
    };

}; // namespace as

#endif // RENDER_PASS_HPP
