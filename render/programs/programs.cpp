#include "programs.hpp"

namespace as
{
    inline void RenderProgramBase::render(vk::Framebuffer framebuffer, std::vector<VirtualObj<Mesh>> meshes)
    {
        render_func(this, framebuffer, meshes);
    };

    RenderProgramBase::~RenderProgramBase()
    {
        for (auto& pipeline : pipelines_)
        {
            if (pipeline)
            {
                DeviceRAII::device_->destroyPipeline(pipeline);
            }
        }

        for (auto& layout : pipeline_layouts_)
        {
            if (layout)
            {
                DeviceRAII::device_->destroyPipelineLayout(layout);
            }
        }

        if (render_pass_)
        {
            DeviceRAII::device_->destroyRenderPass(render_pass_);
        }
    }
} // namespace as