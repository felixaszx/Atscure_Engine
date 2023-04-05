#include "api/cmd.hpp"

as::CmdBuffer::CmdBuffer(const vk::CommandBuffer& buffer, const vk::CommandPool* pool)
    : pool_(pool)
{
    auto_set(*this, buffer);
}

void as::CmdBuffer::free()
{
    device_->freeCommandBuffers(*pool_, *this);
}

as::CmdBuffers::CmdBuffers(const std::vector<vk::CommandBuffer>& buffers, const vk::CommandPool* pool)
    : pool_(pool)
{
    this->resize(buffers.size());
    for (int i = 0; i < buffers.size(); i++)
    {
        this->at(i) = buffers[i];
    }
}

void as::CmdBuffers::free()
{
    device_->freeCommandBuffers(*pool_, *this);
}

as::CmdPool::CmdPool(vk::CommandPoolCreateFlagBits flags)
{
    vk::CommandPoolCreateInfo create_info{};
    create_info.flags = flags;
    create_info.queueFamilyIndex = device_->queue_family_indices_.graphics;
    auto_set(*this, device_->createCommandPool(create_info));
}

as::CmdPool::~CmdPool()
{
    device_->destroyCommandPool(*this);
}

as::CmdBuffer* as::CmdPool::alloc_buffer(vk::CommandBufferLevel level)
{
    vk::CommandBufferAllocateInfo alloc_info{};
    alloc_info.commandBufferCount = 1;
    alloc_info.commandPool = *this;
    alloc_info.level = level;

    return new CmdBuffer(device_->allocateCommandBuffers(alloc_info).front(), this);
}

as::CmdBuffers* as::CmdPool::alloc_buffers(uint32_t count, vk::CommandBufferLevel level)
{
    vk::CommandBufferAllocateInfo alloc_info{};
    alloc_info.commandBufferCount = count;
    alloc_info.commandPool = *this;
    alloc_info.level = level;

    return new CmdBuffers(device_->allocateCommandBuffers(alloc_info), this);
}