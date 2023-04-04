#ifndef AT_MULTITHREAD_HPP
#define AT_MULTITHREAD_HPP

#include <thread>
#include <atomic>
#include <mutex>
#include <semaphore.h>
#include <assert.h>

#include "as_device.hpp"

namespace as
{
    class CpuSemaphore
    {
      private:
        sem_t semaphore_ = nullptr;

      public:
        void create(int initial_value);
        void destroy();

        void signal();
        void wait();
        bool try_wait();
        int get_value();
    };

    struct GpuSemaphore : public MultiType<VkSemaphore>
    {
        void create(VkDevice device);
        void destroy(VkDevice device);
    };

    struct GpuFence : public MultiType<VkFence>
    {
        void create(VkDevice device, bool signal);
        void destroy(VkDevice device);
    };

}; // namespace as

#endif // AT_MULTITHREAD_HPP
