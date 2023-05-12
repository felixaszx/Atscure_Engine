#include "module/render.hpp"

namespace as
{
    struct Renderer::Impl
    {
        std::function<void(const ResultInfo& result,                    //
                           const std::vector<vk::Semaphore>& wait_sems, //
                           const std::vector<vk::Semaphore>& signal_sems)>
            render_func_;

        std::vector<UniqueObj<ImageAttachment>> attachments_;
        std::vector<UniqueObj<DescriptorLayout>> des_layouts_;
        UniqueObj<DescriptorPool> des_pool_{nullptr};

        std::vector<vk::PipelineLayout> pipeline_layouts_{};
        std::vector<vk::Pipeline> pipelines_{};

        std::vector<vk::Framebuffer> fbos_;
    };

    Renderer::~Renderer() {}

    void Renderer::render_scene(const ResultInfo& result,                    //
                                const std::vector<vk::Semaphore>& wait_sems, //
                                const std::vector<vk::Semaphore>& signal_sems)
    {
        impl_->render_func_(result, wait_sems, signal_sems);
    }

    Renderer::ResultInfo Renderer::ResultInfo::swapchain_info(VirtualObj<Swapchain> swapchain, uint32_t index)
    {
        ResultInfo info{};
        info.image_ = *swapchain->images_[index];
        info.format_ = swapchain->format_;
        return info;
    }

    struct RenderModule::Impl
    {
        UniqueObj<Window> window_{nullptr};
        UniqueObj<Context> context_{nullptr};

        UniqueObj<Device> device_{nullptr};
        UniqueObj<Swapchain> swapchain_{nullptr};

        std::vector<UniqueObj<Renderer>> renderers_;
    };

    RenderModule::RenderModule()
        : impl_(new Impl)
    {
        impl_->window_(1920, 1080);
        impl_->context_(true);
        impl_->window_->create_surface(*impl_->context_);

        impl_->device_(*impl_->context_, impl_->context_->VALIDATION_LAYERS);
        impl_->swapchain_(*impl_->window_, *impl_->context_, *impl_->device_);

        window_state_();
    }

    RenderModule::~RenderModule() {}

    bool RenderModule::running()
    {
        return !glfwWindowShouldClose(impl_->window_->window_);
    }

    void RenderModule::poll_events()
    {
        glfwPollEvents();
        window_state_->minimized_ = glfwGetWindowAttrib(impl_->window_->window_, GLFW_ICONIFIED);
    }

    VirtualObj<Renderer> RenderModule::get_renderer(uint32_t index)
    {
        return impl_->renderers_[index];
    }

    void RenderModule::add_renderer(UniqueObj<Renderer>& renderer)
    {
        impl_->renderers_.push_back(std::move(renderer));
    }

}; // namespace as
