#include "deffered.hpp"

namespace as
{
    void gbuffer_func(VirtualObj<CmdBuffer>* target_cmd, VirtualObj<std::atomic_bool> running,
                      VirtualObj<std::binary_semaphore> begin, VirtualObj<std::counting_semaphore<>> end)
    {
        UniqueObj<CmdPool> pool;
        UniqueObj<CmdBuffer> cmd = pool->alloc_buffer(vk::CommandBufferLevel::eSecondary);
        *target_cmd = cmd;

        while (true)
        {
            begin->acquire();
            if (!running->load())
            {
                return;
            }

            end->release();
        }
    }

    DefferedProgram::DefferedProgram()
        : MAX_THREAD_(max_of_all<uint32_t>({std::thread::hardware_concurrency() - 4, 1}))
    {
        main_pool_();
        main_cmd_ = main_pool_->alloc_buffer();
        th_cmds_.resize(MAX_THREAD_);

        for (int i = 0; i < MAX_THREAD_; i++)
        {
            begins_.push_back(new std::binary_semaphore(0));
            ths_.push_back(new std::thread(gbuffer_func, &th_cmds_[i],                    //
                                           VirtualObj<std::atomic_bool>(th_running_),     //
                                           VirtualObj<std::binary_semaphore>(begins_[i]), //
                                           VirtualObj<std::counting_semaphore<>>(ends_)));
        }
    }

    DefferedProgram::~DefferedProgram()
    {
        th_running_ = false;
        for (int i = 0; i < MAX_THREAD_; i++)
        {
            begins_[i]->release();
        }
        for (int i = 0; i < MAX_THREAD_; i++)
        {
            ths_[i]->join();
        }
    }

    void DefferedProgram::run()
    {
        for (int i = 0; i < MAX_THREAD_; i++)
        {
            begins_[i]->release();
        }

        for (int i = 0; i < MAX_THREAD_; i++)
        {
            ends_.acquire();
        }
    }
}; // namespace as