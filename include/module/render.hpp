#ifndef RENDER_HPP
#define RENDER_HPP

#include "as/wrapper.hpp"
#include "vk_api/wrapper.hpp"

namespace as
{
    class RenderModule;
    class Renderer
    {
      public:
        struct ResultInfo;

      protected:
        std::function<void(const ResultInfo& result,                    //
                           const std::vector<vk::Semaphore>& wait_sems, //
                           const std::vector<vk::Semaphore>& signal_sems)>
            render_func_;

      public:
        struct ResultInfo
        {
            uint32_t fbo_index_;
            vk::ImageView view_;
            vk::Format format_;
            vk::ImageLayout initial_layout_;
            vk::ImageLayout final_layout_;
        };

        VirtualObj<Device> device_;
        VirtualObj<Swapchain> swapchain_;

        void render_scene(const ResultInfo& result,                    //
                          const std::vector<vk::Semaphore>& wait_sems, //
                          const std::vector<vk::Semaphore>& signal_sems);

        Renderer(RenderModule& render);
    };

    class RenderModule
    {
        friend Renderer;

      private:
        PIMPL_STRUCT(Impl, impl_);

      public:
        struct WindowState
        {
            std::atomic_bool minimized_;
        };
        WindowState window_state_{};
        UniqueObj<CmdPool> utils_pool_{nullptr};

        RenderModule();
        ~RenderModule();

        bool running();
        void poll_events();

        VirtualObj<Renderer> get_renderer(uint32_t index);
        void add_renderer(VirtualObj<Renderer> renderer);
    };
}; // namespace as

#endif // RENDER_HPP
