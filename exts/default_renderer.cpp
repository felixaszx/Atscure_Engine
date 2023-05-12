#include "exts/default_renderer.hpp"

namespace as
{
    struct DefaultRenderer::Impl
    {
        std::vector<UniqueObj<ImageAttachment>> attachments_;
        std::vector<UniqueObj<DescriptorLayout>> des_layouts_;
        UniqueObj<DescriptorPool> des_pool_{nullptr};

        std::vector<vk::PipelineLayout> pipeline_layouts_{};
        std::vector<vk::Pipeline> pipelines_{};

        std::vector<vk::Framebuffer> fbos_;

        void render_func(const ResultInfo& result,                    //
                         const std::vector<vk::Semaphore>& wait_sems, //
                         const std::vector<vk::Semaphore>& signal_sems){};
    };

    DefaultRenderer::DefaultRenderer(RenderModule& render)
        : Renderer(render),
          impl_(new Impl)
    {
        render_func_ = [this](const ResultInfo& result,                    //
                              const std::vector<vk::Semaphore>& wait_sems, //
                              const std::vector<vk::Semaphore>& signal_sems)
        { impl_->render_func(result, wait_sems, signal_sems); };

        std::vector<vk::Extent2D> extends(6, {1920, 1080});
        std::vector<vk::SampleCountFlagBits> samples(6, vk::SampleCountFlagBits::e1);
        std::vector<vk::Format> formats = {vk::Format::eR32G32B32A32Sfloat, vk::Format::eR32G32B32A32Sfloat,
                                           vk::Format::eR32G32B32A32Sfloat, vk::Format::eR32G32B32A32Sfloat,
                                           vk::Format::eR32G32B32A32Sfloat, vk::Format::eD32SfloatS8Uint};
        std::vector<vk::ImageUsageFlags> usages = {
            vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eInputAttachment, //
            vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eInputAttachment, //
            vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eInputAttachment, //
            vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eInputAttachment, //
            vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eInputAttachment, //
            vk::ImageUsageFlagBits::eDepthStencilAttachment};
        std::vector<vk::ImageAspectFlags> aspects = {vk::ImageAspectFlagBits::eColor, //
                                                     vk::ImageAspectFlagBits::eColor, //
                                                     vk::ImageAspectFlagBits::eColor, //
                                                     vk::ImageAspectFlagBits::eColor, //
                                                     vk::ImageAspectFlagBits::eColor, //
                                                     vk::ImageAspectFlagBits::eDepth |
                                                         vk::ImageAspectFlagBits::eStencil};
        create_image_attachments(impl_->attachments_, formats, extends, samples, usages, aspects);
    }
    DefaultRenderer::~DefaultRenderer() {}
}; // namespace as