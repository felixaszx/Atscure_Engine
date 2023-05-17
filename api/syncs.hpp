#ifndef SYNCS_HPP
#define SYNCS_HPP

#include <semaphore.h>
#include "device.hpp"

namespace as
{
    struct GpuSemaphore : public vk::Semaphore, //
                          public DeviceRAII
    {
        GpuSemaphore();
        ~GpuSemaphore() ;
    };

    struct GpuFence : public vk::Fence, //
                      public DeviceRAII
    {
        GpuFence(bool signal);
        ~GpuFence() ;
    };

}; // namespace as

#endif // SYNCS_HPP
