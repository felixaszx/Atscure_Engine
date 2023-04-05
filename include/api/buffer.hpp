#ifndef BUFFER_HPP
#define BUFFER_HPP

#include "device.hpp"
#include "cmd.hpp"

namespace as
{
    struct Buffer : vk::Buffer, //
                    vk::DeviceMemory,
                    vma::Allocation,
                    public DeviceRAII
    {
      private:
        void* mapping_ = nullptr;

      public:
        const size_t size_ = 0;

        Buffer(const vk::BufferCreateInfo& buffer_info, const vma::AllocationCreateInfo& alloc_info);
        ~Buffer();

        Buffer& copy_from(const Buffer& buffer, CmdPool& pool);

        void* mapping() const;
        void* map_memory();
        void unmap_memory();
    };
}; // namespace as

#endif // BUFFER_HPP