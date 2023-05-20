#ifndef DEFFERED_HPP
#define DEFFERED_HPP

#include <atomic>
#include <thread>
#include <semaphore>
#include "api/wrapper.hpp"
#include "glms.hpp"
#include "mesh.hpp"

namespace as
{
    class DefferedProgram : DeviceRAII
    {
        friend void gbuffer_func(VirtualObj<CmdBuffer>* target_cmd,       //
                                 VirtualObj<std::binary_semaphore> begin, //
                                 DefferedProgram* program);

      private:
        uint32_t width_ = 0;
        uint32_t height_ = 0;

        std::vector<UniqueObj<ImageAttachment>> attachments_{};
        vk::RenderPass render_pass_{};
        vk::Framebuffer framebuffer_{};

        std::vector<UniqueObj<DescriptorLayout>> descriptor_layouts_{};
        UniqueObj<DescriptorPool> descriptor_pool_{nullptr};

        std::vector<vk::PipelineLayout> pipeline_layouts_{};
        std::vector<vk::Pipeline> pipelines_{};

        GpuSemaphore image_sem_{};
        GpuSemaphore submit_sem_{};
        GpuFence frame_fence_{true};

        const uint32_t MAX_THREAD_;
        std::atomic_bool th_running_ = true;
        std::counting_semaphore<> ends_{0};
        std::vector<UniqueObj<std::binary_semaphore>> begins_{};
        std::vector<UniqueObj<std::thread>> ths_{};

        UniqueObj<CmdPool> main_pool_{nullptr};
        UniqueObj<CmdBuffer> main_cmd_{nullptr};
        std::vector<VirtualObj<CmdBuffer>> th_cmds_{};

        struct
        {
            glm::mat4 view_{};
            glm::mat4 proj_{};
        } ubo_;
        vk::DescriptorBufferInfo ubo_info_{};
        vk::WriteDescriptorSet ubo_write_{};
        UniqueObj<Buffer> uniform_buffer_{nullptr};

        void pipeline0();
        void pipeline1();
        void pipeline2();

      public:
        DefferedProgram(uint32_t frame_width, uint32_t frame_height);
        ~DefferedProgram();

        void launch();
        void finish();

        void update_uniform(glm::mat4 view, glm::mat4 proj);
        void record_gbuffer(std::vector<VirtualObj<Mesh>> group);
        void deffered_lighting();
        VirtualObj<Image> result();
    };
}; // namespace as

#endif // DEFFERED_HPP
