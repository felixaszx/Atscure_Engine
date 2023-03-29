#include "at_buffer.hpp"

namespace ats
{
    void Buffer::create(Device& device, VkBufferUsageFlags usage, VkSharingMode sharing, VkDeviceSize size)
    {
        VkBufferCreateInfo buffer_info{};
        buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        buffer_info.usage = usage;
        buffer_info.sharingMode = sharing;
        buffer_info.size = size;
        VmaAllocationCreateInfo alloc_info{};
        alloc_info.usage = memory_;
        alloc_info.flags = allocation_flags_;

        VmaAllocationInfo alloc_result{};
        vmaCreateBuffer(device, &buffer_info, &alloc_info, this->ptr(), this->ptr(), &alloc_result);
        this->set(alloc_result.deviceMemory);
    }

    void* Buffer::create_mapped(Device& device, VkBufferUsageFlags usage, VkSharingMode sharing, VkDeviceSize size)
    {
        create(device, usage, sharing, size);
        void* mapping = nullptr;
        vmaMapMemory(device, *this, &mapping);
        mapped = true;

        return mapping;
    }

    void Buffer::destroy(Device& device)
    {
        if (mapped)
        {
            vmaUnmapMemory(device, *this);
        }

        vmaDestroyBuffer(device, *this, *this);
    }

    void buffer_cpy(Device& device, VkBuffer src, VkBuffer dst, VkBufferCopy region)
    {
        SingleTimeCmd cmd;
        cmd.begin(device);
        vkCmdCopyBuffer(cmd, src, dst, 1, &region);
        cmd.end(device);
    }

    void buffer_cpy(Device& device, VkCommandPool pool, VkBuffer src, VkBuffer dst, VkBufferCopy region)
    {
        SingleTimeCmd cmd;
        cmd.begin(device, pool);
        vkCmdCopyBuffer(cmd, src, dst, 1, &region);
        cmd.end(device, pool);
    }

}; // namespace ats