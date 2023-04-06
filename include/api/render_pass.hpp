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

            void add_image_attachment(vk::AttachmentDescription& attachment,                               //
                                      vk::ImageLayout final_layout = vk::ImageLayout::eUndefined,          //
                                      vk::AttachmentLoadOp load_op = vk::AttachmentLoadOp::eDontCare,      //
                                      vk::AttachmentStoreOp store_op = vk::AttachmentStoreOp::eDontCare,   //
                                      vk::AttachmentLoadOp stencil_load = vk::AttachmentLoadOp::eDontCare, //
                                      vk::AttachmentStoreOp stencil_store = vk::AttachmentStoreOp::eDontCare);
        };

        std::vector<vk::ClearValue> clears_{};

        RenderPass(const RenderPass::Detail& details);
        ~RenderPass() override;
    };

}; // namespace as

#endif // RENDER_PASS_HPP
