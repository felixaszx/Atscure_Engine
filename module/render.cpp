#include "module/render.hpp"

namespace as
{
    struct RenderModule::Impl
    {
        UniqueObj<Window> window_{nullptr};
        UniqueObj<Context> context_{nullptr};

        UniqueObj<Device> device_{nullptr};
        UniqueObj<Swapchain> swapchain_{nullptr};

        std::vector<VirtualObj<Renderer>> renderers_;
    };

    void Renderer::render_scene(const ResultInfo& result,                    //
                                const std::vector<vk::Semaphore>& wait_sems, //
                                const std::vector<vk::Semaphore>& signal_sems)
    {
        render_func_(result, wait_sems, signal_sems);
    }

    Renderer::Renderer(RenderModule& render)
        : device_(render.impl_->device_),
          swapchain_(render.impl_->swapchain_)
    {
    }

    RenderModule::RenderModule()
        : impl_(new Impl)
    {
        impl_->window_(1920, 1080);
        impl_->context_(true);
        impl_->window_->create_surface(*impl_->context_);

        impl_->device_(*impl_->context_, impl_->context_->VALIDATION_LAYERS);
        impl_->swapchain_(*impl_->window_, *impl_->context_, *impl_->device_);

        utils_pool_();
    }

    RenderModule::~RenderModule() {}

    bool RenderModule::running()
    {
        return !glfwWindowShouldClose(impl_->window_->window_);
    }

    void RenderModule::poll_events()
    {
        glfwPollEvents();
        window_state_.minimized_ = glfwGetWindowAttrib(impl_->window_->window_, GLFW_ICONIFIED);
    }

    VirtualObj<Renderer> RenderModule::get_renderer(uint32_t index)
    {
        return impl_->renderers_[index];
    }

    void RenderModule::add_renderer(VirtualObj<Renderer> renderer)
    {
        impl_->renderers_.push_back(renderer);
    }

}; // namespace as
