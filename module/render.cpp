#include "module/render.hpp"

namespace as
{
    struct Renderer::Impl
    {
    };

    Renderer::Renderer() {}
    Renderer::~Renderer() {}

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
    }
    RenderModule::~RenderModule() {}

    bool RenderModule::running()
    {
        return !glfwWindowShouldClose(impl_->window_->window_);
    }

    void RenderModule::poll_events()
    {
        glfwPollEvents();
    }

}; // namespace as
