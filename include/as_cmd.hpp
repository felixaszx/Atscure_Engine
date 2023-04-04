#ifndef AT_CMD_HPP
#define AT_CMD_HPP

#include <functional>

#include "as_device.hpp"
#include "as_multithread.hpp"

namespace as
{
    struct CommandPool : iMultiType(VkCommandPool)
    {
        void create(VkDevice device, uint32_t family_index, VkCommandPoolCreateFlags flags);
        void destroy(VkDevice device);

        VkCommandBuffer allocate_buffer(VkDevice device, VkCommandBufferLevel level);
        std::vector<VkCommandBuffer> allocate_buffers(VkDevice device, uint32_t count, VkCommandBufferLevel level);
    };

    struct SingleTimeCmd : public MultiType<VkCommandPool, VkCommandBuffer>
    {
        void begin(Device& device);
        void end(Device& device);

        void begin(Device& device, VkCommandPool pool);
        void end(Device& device, VkCommandPool pool);
    };

    class MultiThreadCmdRecorder
    {
      private:
        uint32_t curr_cmd = 0;
        VkCommandPool pool_ = VK_NULL_HANDLE;
        std::vector<VkCommandBuffer> cmds_{};
        VkCommandBufferInheritanceInfo inheritance_{};

        std::atomic_bool terminated = false;
        CpuSemaphore begin_semaphore_{};
        CpuSemaphore finish_semaphore_{};

      public:
        void create(Device& device, uint32_t cmd_count = 1);
        void destroy(Device& device);

        VkCommandBuffer get(uint32_t cmd_index = 0);
        void begin(VkCommandBufferInheritanceInfo inheritance, uint32_t cmd_index = 0);
        void wait();
        void wait_than_excute(VkCommandBuffer primary_cmd);
        void terminate();

        void record(const std::function<void(VkCommandBuffer)>& recording_func);
        void operator()(const std::function<void(VkCommandBuffer)>& recording_func);
    };

}; // namespace as

#endif // AT_CMD_HPP
