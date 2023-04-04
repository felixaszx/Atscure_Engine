#ifndef AT_BUFFER_HPP
#define AT_BUFFER_HPP

#include "as_device.hpp"
#include "as_cmd.hpp"

namespace as
{
    class Buffer : iMultiType(VkBuffer, VkDeviceMemory, VmaAllocation)
    {
      private:
        bool mapped = false;
        VmaMemoryUsage memory_ = VMA_MEMORY_USAGE_AUTO;
        VmaAllocationCreateFlagBits allocation_flags_{};

      public:
        size_t size_ = 0;

        Buffer(VmaMemoryUsage memory_usage, VmaAllocationCreateFlagBits allocation_flags);
        void create(Device& device, VkBufferUsageFlags usage, VkSharingMode sharing, VkDeviceSize size);
        void* create_mapped(Device& device, VkBufferUsageFlags usage, VkSharingMode sharing, VkDeviceSize size);
        void* map_memory(Device& device);
        void unmap_memory(Device& device);
        void destroy(Device& device);
    };

    void copy_buffer(Device& device, VkBuffer src, VkBuffer dst, VkBufferCopy region);
    void copy_buffer(Device& device, VkCommandPool pool, VkBuffer src, VkBuffer dst, VkBufferCopy region);

}; // namespace as

#endif // AT_BUFFER_HPP
