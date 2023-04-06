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

void as::RenderPass::Detail::add_image_attachment(vk::AttachmentDescription& attachment, //
                                                  vk::ImageLayout final_layout,          //
                                                  vk::AttachmentLoadOp load_op,          //
                                                  vk::AttachmentStoreOp store_op,        //
                                                  vk::AttachmentLoadOp stencil_load,     //
                                                  vk::AttachmentStoreOp stencil_store)
{
    attachment.finalLayout = final_layout;
    attachment.loadOp = load_op;
    attachment.storeOp = store_op;
    attachment.stencilLoadOp = stencil_load;
    attachment.stencilStoreOp = stencil_store;
    attachments_.push_back(attachment);
}
