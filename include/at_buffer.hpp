#ifndef AT_BUFFER_HPP
#define AT_BUFFER_HPP

#include "at_device.hpp"
#include "at_cmd.hpp"

namespace ats
{
    class Buffer : iMultiType(VkBuffer, VkDeviceMemory, VmaAllocation)
    {
      private:
        bool mapped = false;
        VmaMemoryUsage memory_ = VMA_MEMORY_USAGE_AUTO;
        VmaAllocationCreateFlagBits allocation_flags_ = {};

      public:
        Buffer(VmaMemoryUsage memory_usage, VmaAllocationCreateFlagBits allocation_flags);
        void create(Device& device, VkBufferUsageFlags usage, VkSharingMode sharing, VkDeviceSize size);
        void* create_mapped(Device& device, VkBufferUsageFlags usage, VkSharingMode sharing, VkDeviceSize size);
        void destroy(Device& device);
    };

    void buffer_cpy(Device& device, VkBuffer src, VkBuffer dst, VkBufferCopy region);
    void buffer_cpy(Device& device, VkCommandPool pool, VkBuffer src, VkBuffer dst, VkBufferCopy region);

}; // namespace ats

#endif // AT_BUFFER_HPP
