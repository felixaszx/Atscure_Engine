#ifndef SYNCS_HPP
#define SYNCS_HPP

#include <semaphore.h>
#include "device.hpp"

namespace as
{
    class CpuSemaphore : public DeviceRAII
    {
      private:
        sem_t semaphore_ = nullptr;

      public:
        CpuSemaphore(int initial_value = 0);
        ~CpuSemaphore();

        void signal();
        void wait();
        bool try_wait();
        int get_value();
    };

    struct GpuSemaphore : public vk::Semaphore, //
                          public DeviceRAII
    {
        GpuSemaphore();
        ~GpuSemaphore() override;
    };

    struct GpuFence : public vk::Fence, //
                      public DeviceRAII
    {
        GpuFence(bool signal);
        ~GpuFence() override;
    };

}; // namespace as

#endif // SYNCS_HPP
