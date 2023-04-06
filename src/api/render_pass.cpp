#include "api/render_pass.hpp"

as::RenderPass::RenderPass(const as::RenderPass::Detail& details)
{
    vk::RenderPassCreateInfo create_info{};
    create_info.setSubpasses(details.subpass_);
    create_info.setAttachments(details.attachments_);
    create_info.setDependencies(details.dependency_);
    sset(*this, device_->createRenderPass(create_info));
}

as::RenderPass::~RenderPass()
{
    device_->destroyRenderPass(*this);
}
