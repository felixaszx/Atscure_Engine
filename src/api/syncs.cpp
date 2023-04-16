#include "api/syncs.hpp"

as::CpuSemaphore::CpuSemaphore(int initial_value)
{
    sem_init(&semaphore_, 0, initial_value);
}

as::CpuSemaphore::~CpuSemaphore()
{
    sem_destroy(&semaphore_);
    semaphore_ = nullptr;
}

void as::CpuSemaphore::signal()
{
    sem_post(&semaphore_);
}

void as::CpuSemaphore::wait()
{
    sem_wait(&semaphore_);
}

bool as::CpuSemaphore::try_wait()
{
    return !sem_trywait(&semaphore_);
}

int as::CpuSemaphore::get_value()
{
    int value;
    sem_getvalue(&semaphore_, &value);
    return value;
}

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
