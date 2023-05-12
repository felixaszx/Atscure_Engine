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
    };

    DefaultRenderer::DefaultRenderer(RenderModule& render)
        : Renderer(render),
          impl_(new Impl)
    {
    }
    DefaultRenderer::~DefaultRenderer() {}
}; // namespace as