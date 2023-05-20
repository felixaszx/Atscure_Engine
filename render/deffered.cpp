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

    DefferedProgram::DefferedProgram(uint32_t frame_width, uint32_t frame_height)
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

        std::vector<vk::Extent2D> extends(7, {frame_width, frame_height});
        std::vector<vk::SampleCountFlagBits> samples(7, vk::SampleCountFlagBits::e1);
        std::vector<vk::Format> formats = {vk::Format::eR32G32B32A32Sfloat, vk::Format::eR32G32B32A32Sfloat,
                                           vk::Format::eR32G32B32A32Sfloat, vk::Format::eR32G32B32A32Sfloat,
                                           vk::Format::eR32G32B32A32Sfloat, vk::Format::eD32SfloatS8Uint,
                                           vk::Format::eR32G32B32A32Sfloat};
        std::vector<vk::ImageUsageFlags> usages = {
            vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eInputAttachment, //
            vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eInputAttachment, //
            vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eInputAttachment, //
            vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eInputAttachment, //
            vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eInputAttachment, //
            vk::ImageUsageFlagBits::eDepthStencilAttachment,
            vk::ImageUsageFlagBits::eColorAttachment};
        std::vector<vk::ImageAspectFlags> aspects = {vk::ImageAspectFlagBits::eColor, //
                                                     vk::ImageAspectFlagBits::eColor, //
                                                     vk::ImageAspectFlagBits::eColor, //
                                                     vk::ImageAspectFlagBits::eColor, //
                                                     vk::ImageAspectFlagBits::eColor, //
                                                     vk::ImageAspectFlagBits::eDepth |
                                                         vk::ImageAspectFlagBits::eStencil,
                                                     vk::ImageAspectFlagBits::eColor};
        create_image_attachments(attachments, formats, extends, samples, usages, aspects);

        vk::AttachmentDescription attachment_descriptions[7]{};
        for (uint32_t i = 0; i < 7; i++)
        {
            attachment_descriptions[i].samples = vk::SampleCountFlagBits::e1;
            attachment_descriptions[i].format = vk::Format::eR32G32B32A32Sfloat;
            attachment_descriptions[i].initialLayout = vk::ImageLayout::eUndefined;
            attachment_descriptions[i].finalLayout = vk::ImageLayout::eColorAttachmentOptimal;
            attachment_descriptions[i].loadOp = vk::AttachmentLoadOp::eClear;
            attachment_descriptions[i].storeOp = vk::AttachmentStoreOp::eDontCare;
            attachment_descriptions[i].stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
            attachment_descriptions[i].stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
        }
        attachment_descriptions[5].finalLayout = vk::ImageLayout::eDepthStencilAttachmentOptimal;
        attachment_descriptions[5].format = vk::Format::eD32SfloatS8Uint;
        attachment_descriptions[6].format = vk::Format::eR32G32B32A32Sfloat;
        attachment_descriptions[6].storeOp = vk::AttachmentStoreOp::eStore;

        vk::AttachmentReference attachment_references0[5]{};
        vk::AttachmentReference attachment_references1[5]{};
        vk::AttachmentReference attachment_references2[2]{};

        // first subpass attachment
        for (uint32_t i = 0; i < 4; i++)
        {
            attachment_references0[i].attachment = i;
            attachment_references0[i].layout = vk::ImageLayout::eColorAttachmentOptimal;
        }
        attachment_references0[4].attachment = 5;
        attachment_references0[4].layout = vk::ImageLayout::eDepthStencilAttachmentOptimal;

        // second subpass attachment
        for (uint32_t i = 0; i < 5; i++)
        {
            attachment_references1[i].attachment = i;
            attachment_references1[i].layout = vk::ImageLayout::eShaderReadOnlyOptimal;
        }
        attachment_references1[4].layout = vk::ImageLayout::eColorAttachmentOptimal;

        // third subpass attachment
        attachment_references2[0].attachment = 4;
        attachment_references2[0].layout = vk::ImageLayout::eShaderReadOnlyOptimal;
        attachment_references2[1].attachment = 6;
        attachment_references2[1].layout = vk::ImageLayout::eColorAttachmentOptimal;

        vk::SubpassDescription subpasses[3]{};
        subpasses[0].colorAttachmentCount = 4;
        subpasses[0].pColorAttachments = attachment_references0;
        subpasses[0].pDepthStencilAttachment = attachment_references0 + 4;

        subpasses[1].colorAttachmentCount = 1;
        subpasses[1].pColorAttachments = attachment_references1 + 4;
        subpasses[1].inputAttachmentCount = 4;
        subpasses[1].pInputAttachments = attachment_references1;

        subpasses[2].colorAttachmentCount = 1;
        subpasses[2].pColorAttachments = attachment_references2 + 1;
        subpasses[2].inputAttachmentCount = 1;
        subpasses[2].pInputAttachments = attachment_references2;

        vk::SubpassDependency dependencies[2]{};
        dependencies[0].srcSubpass = 0;
        dependencies[0].dstSubpass = 1;
        dependencies[0].srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput | //
                                       vk::PipelineStageFlagBits::eLateFragmentTests;
        dependencies[0].dstStageMask = vk::PipelineStageFlagBits::eFragmentShader;
        dependencies[0].srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite | //
                                        vk::AccessFlagBits::eDepthStencilAttachmentWrite;
        dependencies[0].dstAccessMask = vk::AccessFlagBits::eInputAttachmentRead;

        dependencies[1].srcSubpass = 1;
        dependencies[1].dstSubpass = 2;
        dependencies[1].srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
        dependencies[1].dstStageMask = vk::PipelineStageFlagBits::eFragmentShader;
        dependencies[1].srcAccessMask = vk::AccessFlagBits::eColorAttachmentWrite;
        dependencies[1].dstAccessMask = vk::AccessFlagBits::eInputAttachmentRead;

        vk::RenderPassCreateInfo render_pass_info{};
        render_pass_info.setSubpasses(subpasses);
        render_pass_info.setAttachments(attachment_descriptions);
        render_pass_info.setDependencies(dependencies);
        render_pass_ = device_->createRenderPass(render_pass_info);
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