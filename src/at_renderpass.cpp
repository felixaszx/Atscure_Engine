#include "at_renderpass.hpp"

namespace ats
{
    const VkAttachmentDescription RenderPassAttachments::COLOR_ATTACHMENT = //
        {0,
         VK_FORMAT_R32G32B32A32_SFLOAT,
         VK_SAMPLE_COUNT_1_BIT,
         VK_ATTACHMENT_LOAD_OP_CLEAR,
         VK_ATTACHMENT_STORE_OP_DONT_CARE,
         VK_ATTACHMENT_LOAD_OP_DONT_CARE,
         VK_ATTACHMENT_STORE_OP_DONT_CARE,
         VK_IMAGE_LAYOUT_UNDEFINED,
         VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};
    const VkAttachmentDescription RenderPassAttachments::DEPTH_STENCIL_ATTACHMENT = //
        {0,
         VK_FORMAT_D32_SFLOAT_S8_UINT,
         VK_SAMPLE_COUNT_1_BIT,
         VK_ATTACHMENT_LOAD_OP_CLEAR,
         VK_ATTACHMENT_STORE_OP_DONT_CARE,
         VK_ATTACHMENT_LOAD_OP_DONT_CARE,
         VK_ATTACHMENT_STORE_OP_DONT_CARE,
         VK_IMAGE_LAYOUT_UNDEFINED,
         VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL};

    RenderPassAttachments::RenderPassAttachments(VkSampleCountFlagBits samples)
        : SAMPLE_COUNT(samples)
    {
    }

    RenderPassAttachments::operator VkAttachmentDescription*()
    {
        return descriptions_.data();
    }

    void RenderPassAttachments::add(VkAttachmentDescription attachment)
    {
        attachment.samples = SAMPLE_COUNT;
        descriptions_.push_back(attachment);
    }

    uint32_t RenderPassAttachments::size()
    {
        return descriptions_.size();
    }
}; // namespace ats