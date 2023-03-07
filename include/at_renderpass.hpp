#ifndef AT_RENDERPASS_HPP
#define AT_RENDERPASS_HPP

#include "at_device.hpp"
#include "at_image.hpp"
#include "at_shader.hpp"

namespace ats
{
    class SubpassAttachmentRef
    {
      private:
      public:
    };

    class RenderPassAttachments
    {
      private:
        std::vector<VkAttachmentDescription> descriptions_{};

      public:
        const VkSampleCountFlagBits SAMPLE_COUNT = VK_SAMPLE_COUNT_1_BIT;
        static const VkAttachmentDescription COLOR_ATTACHMENT;
        static const VkAttachmentDescription DEPTH_STENCIL_ATTACHMENT;

        RenderPassAttachments(VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT);
        operator VkAttachmentDescription*();

        void add(VkAttachmentDescription attachment);
        uint32_t size();
    };

    struct Subpass : iMultiType(VkSubpassDescription)
    {
    };

    struct RenderPass : iMultiType(VkRenderPass)
    {
    };
}; // namespace ats

#endif // AT_RENDERPASS_HPP
