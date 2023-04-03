#include "at_cmd.hpp"

namespace ats
{

    void CommandPool::create(VkDevice device, uint32_t family_index, VkCommandPoolCreateFlags flags)
    {
        VkCommandPoolCreateInfo pool_info{};
        pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        pool_info.queueFamilyIndex = family_index;
        vkCreateCommandPool(device, &pool_info, nullptr, this->ptr());
    }

    void CommandPool::destroy(VkDevice device)
    {
        vkDestroyCommandPool(device, *this, nullptr);
    }

    VkCommandBuffer CommandPool::allocate_buffer(VkDevice device, VkCommandBufferLevel level)
    {
        VkCommandBufferAllocateInfo cmd_alloc_info{};
        cmd_alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        cmd_alloc_info.commandPool = *this;
        cmd_alloc_info.level = level;
        cmd_alloc_info.commandBufferCount = 1;

        VkCommandBuffer cmd;
        vkAllocateCommandBuffers(device, &cmd_alloc_info, &cmd);
        return cmd;
    }

    std::vector<VkCommandBuffer> CommandPool::allocate_buffers(VkDevice device, uint32_t count,
                                                               VkCommandBufferLevel level)
    {
        VkCommandBufferAllocateInfo cmd_alloc_info{};
        cmd_alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        cmd_alloc_info.commandPool = *this;
        cmd_alloc_info.level = level;
        cmd_alloc_info.commandBufferCount = count;

        std::vector<VkCommandBuffer> cmds(count);
        vkAllocateCommandBuffers(device, &cmd_alloc_info, cmds.data());
        return cmds;
    }

    void SingleTimeCmd::begin(Device& device)
    {
        VkCommandPoolCreateInfo pool_info{};
        pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        pool_info.queueFamilyIndex = device.queue_family_indices_.graphics;
        vkCreateCommandPool(device, &pool_info, nullptr, this->ptr());

        begin(device, *this);
    }

    void SingleTimeCmd::end(Device& device)
    {
        end(device, *this);
        vkDestroyCommandPool(device, *this, nullptr);
    }

    void SingleTimeCmd::begin(Device& device, VkCommandPool pool)
    {
        VkCommandBufferAllocateInfo alloc_info{};
        alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        alloc_info.commandPool = pool;
        alloc_info.commandBufferCount = 1;
        vkAllocateCommandBuffers(device, &alloc_info, this->ptr());

        VkCommandBufferBeginInfo begin_info{};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        vkBeginCommandBuffer(*this, &begin_info);
    }

    void SingleTimeCmd::end(Device& device, VkCommandPool pool)
    {
        vkEndCommandBuffer(*this);

        VkSubmitInfo submit_info{};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = this->ptr();

        vkQueueSubmit(device.graphics_queue_, 1, &submit_info, VK_NULL_HANDLE);
        vkQueueWaitIdle(device.graphics_queue_);

        vkFreeCommandBuffers(device, pool, 1, this->ptr());
    }

    void MultiThreadCmdRecorder::create(Device& device, uint32_t cmd_count)
    {
        VkCommandPoolCreateInfo pool_info{};
        pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        pool_info.queueFamilyIndex = device.queue_family_indices_.graphics;
        vkCreateCommandPool(device, &pool_info, nullptr, &pool_);

        cmds_.resize(cmd_count);
        VkCommandBufferAllocateInfo alloc_info{};
        alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        alloc_info.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
        alloc_info.commandPool = pool_;
        alloc_info.commandBufferCount = cmd_count;
        vkAllocateCommandBuffers(device, &alloc_info, cmds_.data());

        begin_semaphore_.create(0);
        finish_semaphore_.create(0);
    }

    void MultiThreadCmdRecorder::destroy(Device& device)
    {
        begin_semaphore_.destroy();
        finish_semaphore_.destroy();
        vkDestroyCommandPool(device, pool_, nullptr);
    }

    VkCommandBuffer MultiThreadCmdRecorder::get(uint32_t cmd_index)
    {
        return cmds_[cmd_index];
    }

    void MultiThreadCmdRecorder::begin(VkCommandBufferInheritanceInfo inheritance, uint32_t cmd_index)
    {
        inheritance_ = inheritance;
        curr_cmd = cmd_index;
        begin_semaphore_.signal();
    }

    void MultiThreadCmdRecorder::wait()
    {
        finish_semaphore_.wait();
    }

    void MultiThreadCmdRecorder::wait_than_excute(VkCommandBuffer primary_cmd)
    {
        wait();
        vkCmdExecuteCommands(primary_cmd, 1, &cmds_[curr_cmd]);
    }

    void MultiThreadCmdRecorder::terminate()
    {
        terminated = true;
        begin({});
    }

    void MultiThreadCmdRecorder::record(const std::function<void(VkCommandBuffer)>& recording_func)
    {
        while (true)
        {
            begin_semaphore_.wait();
            if (terminated)
            {
                return;
            }

            if (curr_cmd < cmds_.size())
            {
                vkResetCommandBuffer(cmds_[curr_cmd], 0);
                VkCommandBufferBeginInfo begin_info{};
                begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
                begin_info.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
                begin_info.pInheritanceInfo = &inheritance_;

                if (vkBeginCommandBuffer(cmds_[curr_cmd], &begin_info) != VK_SUCCESS)
                {
                    Log::error("This secondary commandbuffer do not start");
                }

                recording_func(cmds_[curr_cmd]);

                if (vkEndCommandBuffer(cmds_[curr_cmd]) != VK_SUCCESS)
                {
                    Log::error("This secondary commandbuffer do not end");
                }
            }

            finish_semaphore_.signal();
        }
    }

    void MultiThreadCmdRecorder::operator()(const std::function<void(VkCommandBuffer)>& recording_func)
    {
        record(recording_func);
    }

}; // namespace ats