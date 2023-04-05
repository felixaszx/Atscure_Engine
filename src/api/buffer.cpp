#include "api/buffer.hpp"

as::Buffer::Buffer(const vk::BufferCreateInfo& buffer_info, const vma::AllocationCreateInfo& alloc_info)
    : size_(buffer_info.size)
{
    vma::AllocationInfo finish_info{};
    sset(*this, device_->allocator_.createBuffer(buffer_info, alloc_info, finish_info).first);
    sset(*this, finish_info.deviceMemory);
}

as::Buffer::~Buffer()
{
    unmap_memory();
    device_->allocator_.destroyBuffer(*this, *this);
}

as::Buffer& as::Buffer::copy_from(const Buffer& buffer, CmdPool& pool)
{
    vk::BufferCopy region{};
    region.size = std::min(this->size_, buffer.size_);

    CmdBuffer cmd = *pool.alloc_buffer();
    vk::CommandBufferBeginInfo begin_info{};
    begin_info.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
    cmd.begin(begin_info);
    cmd.copyBuffer(buffer, *this, region);
    cmd.end();

    vk::SubmitInfo submit_info{};
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &cmd;

    device_->graphics_queue_.submit(submit_info);
    device_->graphics_queue_.waitIdle();
    return *this;
}

void* as::Buffer::mapping() const
{
    return mapping_;
}

void* as::Buffer::map_memory()
{
    mapping_ = device_->allocator_.mapMemory(*this);
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
