#ifndef RENDER_HPP
#define RENDER_HPP

#include "as/wrapper.hpp"
#include "vk_api/wrapper.hpp"

namespace as
{
    class Renderer
    {
      private:
        PIMPL_STRUCT(Impl, impl_);

      public:
        struct ResultInfo
        {
            vk::ImageView image_;
            vk::Format format_;
            vk::ImageLayout initial_layout_;
            vk::ImageLayout final_layout_;

            static ResultInfo swapchain_info(VirtualObj<Swapchain> swapchain, uint32_t index);
        };

        ~Renderer();

        void render_scene(const ResultInfo& result,                    //
                          const std::vector<vk::Semaphore>& wait_sems, //
                          const std::vector<vk::Semaphore>& signal_sems);
    };

    class RenderModule
    {
      private:
        PIMPL_STRUCT(Impl, impl_);

      public:
        struct WindowState
        {
            std::atomic_bool minimized_;
        };
        WindowState window_state_{};

        RenderModule();
        ~RenderModule();

        bool running();
        void poll_events();

        VirtualObj<Renderer> get_renderer(uint32_t index);
        void add_renderer(UniqueObj<Renderer>& renderer);
    };
}; // namespace as

#endif // RENDER_HPP
