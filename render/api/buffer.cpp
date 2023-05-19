#include "buffer.hpp"

as::Buffer::Buffer(const vk::BufferCreateInfo& buffer_info, const vma::AllocationCreateInfo& alloc_info)
    : size_(buffer_info.size)
{
    vma::AllocationInfo finish_info{};
    auto alloc_result = device_->allocator_.createBuffer(buffer_info, alloc_info, finish_info);
    sset(*this, alloc_result.first, alloc_result.second);
    sset(*this, finish_info.deviceMemory);
}

as::Buffer::~Buffer()
{
    unmap_memory();
    device_->allocator_.destroyBuffer(*this, *this);
}

void as::Buffer::copy_from(VirtualObj<Buffer> buffer, VirtualObj<CmdPool> pool, const vk::BufferCopy& region)
{
    UniqueObj<CmdBuffer> cmd = pool->alloc_buffer();
    as::begin_cmd(cmd, vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
    cmd->copyBuffer(buffer, *this, region);
    cmd->end();

    vk::SubmitInfo submit_info{};
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = cmd.get();

    device_->graphics_queue_.submit(submit_info);
    device_->graphics_queue_.waitIdle();
}

void* as::Buffer::mapping() const
{
    return mapping_;
}

void* as::Buffer::map_memory()
{
    if (mapping_ == nullptr)
    {
        mapping_ = device_->allocator_.mapMemory(*this);
        return mapping_;
    }
    return mapping_;
}

void as::Buffer::unmap_memory()
{
    if (mapping_ != nullptr)
    {
        device_->allocator_.unmapMemory(*this);
        mapping_ = nullptr;
    }
}
