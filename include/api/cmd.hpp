#ifndef CMD_HPP
#define CMD_HPP

#include "device.hpp"

namespace as
{
    struct CmdBuffer : public vk::CommandBuffer, //
                       public DeviceRAII
    {
      private:
        const vk::CommandPool* pool_{};

      public:
        CmdBuffer(const vk::CommandBuffer& buffer, const vk::CommandPool* pool);
        ~CmdBuffer();
    };

    struct CmdBuffers : std::vector<vk::CommandBuffer>, //
                        public DeviceRAII
    {
      private:
        const vk::CommandPool* pool_{};

      public:
        using std::vector<vk::CommandBuffer>::size;
        using std::vector<vk::CommandBuffer>::operator[];

        CmdBuffers(const std::vector<vk::CommandBuffer>& buffers, const vk::CommandPool* pool);
        ~CmdBuffers();
    };

    struct CmdPool : public vk::CommandPool, //
                     public DeviceRAII
    {
        CmdPool(vk::CommandPoolCreateFlagBits flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer);
        ~CmdPool();

        CmdBuffer* alloc_buffer(vk::CommandBufferLevel level = vk::CommandBufferLevel::ePrimary);
        CmdBuffers* alloc_buffers(uint32_t count, vk::CommandBufferLevel level = vk::CommandBufferLevel::ePrimary);
    };

}; // namespace as

#endif // CMD_HPP
