#include "vk_api/syncs.hpp"

as::GpuSemaphore::GpuSemaphore()
{
    vk::SemaphoreCreateInfo create_info{};
    sset(*this, device_->createSemaphore(create_info));
}

as::GpuSemaphore::~GpuSemaphore()
{
    device_->destroySemaphore(*this);
}

as::GpuFence::GpuFence(bool signal)
{
    vk::FenceCreateInfo fence_info{};
    if (signal)
    {
        fence_info.flags = vk::FenceCreateFlagBits::eSignaled;
    }
    sset(*this, device_->createFence(fence_info));
}

as::GpuFence::~GpuFence()
{
    device_->destroyFence(*this);
}
