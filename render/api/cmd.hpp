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

    void begin_cmd(VirtualObj<vk::CommandBuffer> cmd,      //
                   vk::CommandBufferUsageFlags flags = {}, //
                   vk::CommandBufferInheritanceInfo inheritance = {});

    struct CmdPool : public vk::CommandPool, //
                     public DeviceRAII
    {
        CmdPool(vk::CommandPoolCreateFlagBits flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer);
        ~CmdPool();

        BridgeObj<CmdBuffer> alloc_buffer(vk::CommandBufferLevel level = vk::CommandBufferLevel::ePrimary);
        BridgeObj<CmdBuffers> alloc_buffers(uint32_t count,
                                            vk::CommandBufferLevel level = vk::CommandBufferLevel::ePrimary);

        void submit_onetime(CmdBuffer* buffer);
        void submit_onetime(CmdBuffers* buffer);
    };

}; // namespace as

#endif // CMD_HPP
