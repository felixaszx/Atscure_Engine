#include <iostream>
#include <string>

#include "as_window.hpp"
#include "as_device.hpp"
#include "as_swapchain.hpp"
#include "as_shader.hpp"
#include "as_image.hpp"
#include "as_pipeline.hpp"
#include "as_mesh.hpp"
#include "as_cmd.hpp"
#include "as_buffer.hpp"
#include "as_camera.hpp"

#include "as_texture.hpp"
#include "as_mesh.hpp"
#include "as_light.hpp"

int main(int argc, char** argv)
{
    as::Instance instance{};
    as::WindowManager window(1920, 1080);
    window.create("test app", instance, false);

    as::Device device(instance, instance);
    device.create(instance, instance.VALIDATION_LAYERS);

    as::Swapchain swapchain;
    swapchain.create(window, instance, device);
    swapchain.create_image_view();

    std::vector<VkExtent2D> extends(6, swapchain.extend_);
    std::vector<VkSampleCountFlagBits> samples(6, VK_SAMPLE_COUNT_1_BIT);
    std::vector<VkFormat> formats = {VK_FORMAT_R32G32B32A32_SFLOAT, VK_FORMAT_R32G32B32A32_SFLOAT,
                                     VK_FORMAT_R32G32B32A32_SFLOAT, VK_FORMAT_R32G32B32A32_SFLOAT,
                                     VK_FORMAT_R32G32B32A32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT};
    std::vector<VkImageUsageFlags> usages = {
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT, //
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT, //
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT, //
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT, //
        VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT, //
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT};
    std::vector<VkImageAspectFlags> aspects = {VK_IMAGE_ASPECT_COLOR_BIT, //
                                               VK_IMAGE_ASPECT_COLOR_BIT, //
                                               VK_IMAGE_ASPECT_COLOR_BIT, //
                                               VK_IMAGE_ASPECT_COLOR_BIT, //
                                               VK_IMAGE_ASPECT_COLOR_BIT, //
                                               VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT};
    std::vector<as::ImageAttachment> attachments = as::create_image_attachments(device,           //
                                                                                  formats, extends, //
                                                                                  samples, usages,  //
                                                                                  aspects);

    VkAttachmentDescription attachment_descriptions[7]{};
    for (uint32_t i = 0; i < 7; i++)
    {
        attachment_descriptions[i].samples = VK_SAMPLE_COUNT_1_BIT;
        attachment_descriptions[i].format = VK_FORMAT_R32G32B32A32_SFLOAT;
        attachment_descriptions[i].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        attachment_descriptions[i].finalLayout = VK_IMAGE_LAYOUT_GENERAL;
        attachment_descriptions[i].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachment_descriptions[i].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachment_descriptions[i].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachment_descriptions[i].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    }
    attachment_descriptions[5].finalLayout = VK_IMAGE_LAYOUT_GENERAL;
    attachment_descriptions[5].format = VK_FORMAT_D32_SFLOAT_S8_UINT;
    attachment_descriptions[6].format = swapchain.format_;
    attachment_descriptions[6].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    attachment_descriptions[6].storeOp = VK_ATTACHMENT_STORE_OP_STORE;

    VkAttachmentReference attachment_references0[5]{};
    VkAttachmentReference attachment_references1[5]{};
    VkAttachmentReference attachment_references2[2]{};

    // first subpass attachment
    for (uint32_t i = 0; i < 4; i++)
    {
        attachment_references0[i].attachment = i;
        attachment_references0[i].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    }
    attachment_references0[4].attachment = 5;
    attachment_references0[4].layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    // second subpass attachment
    for (uint32_t i = 0; i < 5; i++)
    {
        attachment_references1[i].attachment = i;
        attachment_references1[i].layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    }
    attachment_references1[4].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    // third subpass attachment
    attachment_references2[0].attachment = 4;
    attachment_references2[0].layout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    attachment_references2[1].attachment = 6;
    attachment_references2[1].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpasses[3]{};
    subpasses[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpasses[0].colorAttachmentCount = 4;
    subpasses[0].pColorAttachments = attachment_references0;
    subpasses[0].pDepthStencilAttachment = attachment_references0 + 4;

    subpasses[1].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpasses[1].colorAttachmentCount = 1;
    subpasses[1].pColorAttachments = attachment_references1 + 4;
    subpasses[1].inputAttachmentCount = 4;
    subpasses[1].pInputAttachments = attachment_references1;

    subpasses[2].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpasses[2].colorAttachmentCount = 1;
    subpasses[2].pColorAttachments = attachment_references2 + 1;
    subpasses[2].inputAttachmentCount = 1;
    subpasses[2].pInputAttachments = attachment_references2;

    VkSubpassDependency dependencies[2]{};
    dependencies[0].srcSubpass = 0;
    dependencies[0].dstSubpass = 1;
    dependencies[0].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | //
                                   VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
    dependencies[0].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    dependencies[0].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | //
                                    VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    dependencies[0].dstAccessMask = VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;

    dependencies[1].srcSubpass = 1;
    dependencies[1].dstSubpass = 2;
    dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dependencies[1].dstAccessMask = VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;

    VkRenderPassCreateInfo render_pass_info{};
    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    render_pass_info.subpassCount = 3;
    render_pass_info.pSubpasses = subpasses;
    render_pass_info.attachmentCount = 7;
    render_pass_info.pAttachments = attachment_descriptions;
    render_pass_info.dependencyCount = 2;
    render_pass_info.pDependencies = dependencies;
    VkRenderPass render_pass;
    vkCreateRenderPass(device, &render_pass_info, nullptr, &render_pass);

    as::DescriptorLayout layouts[3]{};
    as::PipelineLayout pipeline_layouts[3]{};
    layouts[0].add_binding(0, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);
    layouts[0].add_binding(1, 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);
    for (int i = 0; i < 4; i++)
    {
        layouts[1].add_binding(i, 1, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, VK_SHADER_STAGE_FRAGMENT_BIT);
    }
    layouts[2].add_binding(0, 1, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, VK_SHADER_STAGE_FRAGMENT_BIT);
    for (int i = 0; i < 3; i++)
    {
        layouts[i].create(device, VK_DESCRIPTOR_SET_LAYOUT_CREATE_PUSH_DESCRIPTOR_BIT_KHR);
    }
    for (int i = 0; i < 3; i++)
    {
        pipeline_layouts[i].add_layout(layouts[i]);
        pipeline_layouts[i].create(device);
    }

    auto binding_description = as::Mesh::get_bindings();
    auto attribute_description = as::Mesh::get_attributes();
    as::ShaderModule verts[3]{};
    as::ShaderModule frags[3]{};
    for (int i = 0; i < 3; i++)
    {
        verts[i].create(device, as::ShaderModule::read_file("res/shader/vert" + std::to_string(i) + ".spv"), "main",
                        VK_SHADER_STAGE_VERTEX_BIT);
        frags[i].create(device, as::ShaderModule::read_file("res/shader/frag" + std::to_string(i) + ".spv"), "main",
                        VK_SHADER_STAGE_FRAGMENT_BIT);
    }

    as::GraphicPipeline pipelines[3]{};
    for (int i = 0; i < 3; i++)
    {
        pipelines[i].stages_.set_vert_shader(verts[i].stage_info_);
        pipelines[i].stages_.set_frag_shader(frags[i].stage_info_);
    }

    VkPipelineColorBlendAttachmentState color_blend_attachment0{};
    color_blend_attachment0.colorWriteMask = as::ColorWrite::RGBA;
    pipelines[0].set_vertex_states(binding_description, attribute_description);
    pipelines[0].set_view_port_state(1, 1);
    pipelines[0].set_rasterizer(VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT);
    pipelines[0].add_color_blending(color_blend_attachment0);
    pipelines[0].add_color_blending(color_blend_attachment0);
    pipelines[0].add_color_blending(color_blend_attachment0);
    pipelines[0].add_color_blending(color_blend_attachment0);
    pipelines[0].set_depth_stencil(true, false);
    pipelines[0].create(device, pipeline_layouts[0], render_pass, 0);

    VkPipelineColorBlendAttachmentState color_blend_attachment1{};
    color_blend_attachment1.blendEnable = VK_TRUE;
    color_blend_attachment1.colorBlendOp = VK_BLEND_OP_ADD;
    color_blend_attachment1.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    color_blend_attachment1.dstColorBlendFactor = VK_BLEND_FACTOR_ONE;
    color_blend_attachment1.colorWriteMask = as::ColorWrite::RGBA;
    pipelines[1].set_view_port_state(1, 1);
    pipelines[1].set_rasterizer(VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE);
    pipelines[1].add_color_blending(color_blend_attachment1);
    pipelines[1].set_depth_stencil(false, false);
    pipelines[1].create(device, pipeline_layouts[1], render_pass, 1);

    VkPipelineColorBlendAttachmentState color_blend_attachment2{};
    color_blend_attachment2.colorWriteMask = as::ColorWrite::RGBA;
    pipelines[2].set_view_port_state(1, 1);
    pipelines[2].set_rasterizer(VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE);
    pipelines[2].add_color_blending(color_blend_attachment2);
    pipelines[2].set_depth_stencil(false, false);
    pipelines[2].create(device, pipeline_layouts[2], render_pass, 2);

    as::CommandPool cmd_pool;
    cmd_pool.create(device, device.queue_family_indices_.graphics, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    VkCommandBuffer cmd = cmd_pool.allocate_buffer(device, VK_COMMAND_BUFFER_LEVEL_PRIMARY);

    as::Texture tt = as::create_image_texture(device, cmd_pool, "res/textures/ayaka.png");
    tt.load(device, cmd_pool);
    tt.create_image_view(device);

    as::Sampler sampler;
    sampler.set_adress_mode(VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK);
    sampler.set_anisotropy(4);
    sampler.set_compare(VK_FALSE);
    sampler.set_filter(VK_FILTER_LINEAR, VK_FILTER_LINEAR);
    sampler.set_mipmap(VK_SAMPLER_MIPMAP_MODE_LINEAR, 0, 0, 0);
    sampler.create(device);

    as::GpuSemaphore image_semaphore;
    as::GpuSemaphore submit_semaphore;
    as::GpuFence frame_fence;
    image_semaphore.create(device);
    submit_semaphore.create(device);
    frame_fence.create(device, true);

    std::vector<VkFramebuffer> framebuffers(swapchain.image_views_.size());
    for (int i = 0; i < swapchain.image_views_.size(); i++)
    {
        VkImageView fattachments[] = {attachments[0],           attachments[1], attachments[2],
                                      attachments[3],           attachments[4], attachments[5], //
                                      swapchain.image_views_[i]};
        VkFramebufferCreateInfo fcreate_info{};
        fcreate_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        fcreate_info.renderPass = render_pass;
        fcreate_info.attachmentCount = 7;
        fcreate_info.pAttachments = fattachments;
        fcreate_info.width = swapchain.extend_.width;
        fcreate_info.height = swapchain.extend_.height;
        fcreate_info.layers = 1;
        vkCreateFramebuffer(device, &fcreate_info, nullptr, &framebuffers[i]);
    }

    VkViewport viewport{};
    viewport.width = casts(uint32_t, swapchain.extend_.width);
    viewport.height = casts(uint32_t, swapchain.extend_.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor{};
    scissor.extent = swapchain.extend_;

    as::Mesh aa("res/model/sponza/sponza.obj", 1);
    aa.create(device);

    as::Camera camera;
    camera.create(device);
    camera.position_ = {0, 20, 0};

    VkDescriptorImageInfo descriptor_image_infos[6]{};
    for (int i = 0; i < 5; i++)
    {
        descriptor_image_infos[i].sampler = VK_NULL_HANDLE;
        descriptor_image_infos[i].imageView = attachments[i];
        descriptor_image_infos[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    }
    descriptor_image_infos[5].sampler = sampler;
    descriptor_image_infos[5].imageView = tt;
    descriptor_image_infos[5].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    VkWriteDescriptorSet image_write{};
    image_write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    image_write.dstBinding = 0;
    image_write.descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
    image_write.descriptorCount = 4;
    image_write.pImageInfo = descriptor_image_infos;
    VkWriteDescriptorSet image_write2{};
    image_write2.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    image_write2.dstBinding = 0;
    image_write2.descriptorType = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
    image_write2.descriptorCount = 1;
    image_write2.pImageInfo = descriptor_image_infos + 4;
    VkWriteDescriptorSet image_write3{};
    image_write3.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    image_write3.dstBinding = 1;
    image_write3.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    image_write3.descriptorCount = 1;
    image_write3.pImageInfo = descriptor_image_infos + 5;

    as::MultiThreadCmdRecorder recorder[3]{};
    for (int i = 0; i < 3; i++)
    {
        recorder[i].create(device);
    }

    std::thread record_th0(std::ref(recorder[0]),
                           [&](VkCommandBuffer scmd)
                           {
                               VkWriteDescriptorSet write{};
                               write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                               write.dstBinding = 0;
                               write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                               write.descriptorCount = 1;
                               write.pBufferInfo = &camera.buffer_info_;
                               VkWriteDescriptorSet writes[2]{write, image_write3};
                               vkCmdBindPipeline(scmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelines[0]);
                               as::Device::CmdPushDescriptorSetKHR(scmd, VK_PIPELINE_BIND_POINT_GRAPHICS, //
                                                                    pipeline_layouts[0], 0, 2, writes);
                               vkCmdSetViewport(scmd, 0, 1, &viewport);
                               vkCmdSetScissor(scmd, 0, 1, &scissor);

                               aa.models_[0] = glm::scale(glm::mat4(1.0f), {0.1, 0.1, 0.1});
                               aa.update();
                               aa.draw(scmd);
                           });

    std::thread record_th1(std::ref(recorder[1]),
                           [&](VkCommandBuffer scmd)
                           {
                               vkCmdBindPipeline(scmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelines[1]);
                               as::Device::CmdPushDescriptorSetKHR(scmd, VK_PIPELINE_BIND_POINT_GRAPHICS, //
                                                                    pipeline_layouts[1], 0, 1, &image_write);
                               vkCmdSetViewport(scmd, 0, 1, &viewport);
                               vkCmdSetScissor(scmd, 0, 1, &scissor);
                               vkCmdDraw(scmd, 6, 1, 0, 0);
                           });

    std::thread record_th2(std::ref(recorder[2]),
                           [&](VkCommandBuffer scmd)
                           {
                               vkCmdBindPipeline(scmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelines[2]);
                               as::Device::CmdPushDescriptorSetKHR(scmd, VK_PIPELINE_BIND_POINT_GRAPHICS, //
                                                                    pipeline_layouts[2], 0, 1, &image_write2);
                               vkCmdSetViewport(scmd, 0, 1, &viewport);
                               vkCmdSetScissor(scmd, 0, 1, &scissor);
                               vkCmdDraw(scmd, 6, 1, 0, 0);
                           });

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        camera.update(swapchain.extend_);

        auto result = vkWaitForFences(device, 1, &frame_fence, VK_TRUE, UINT64_MAX);
        if (result != VK_SUCCESS)
        {
            throw std::runtime_error("Fence error\n");
        }
        uint32_t image_index = 0;
        vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, image_semaphore, VK_NULL_HANDLE, &image_index);
        vkResetFences(device, 1, &frame_fence);

        VkCommandBufferInheritanceInfo iifo{};
        iifo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
        iifo.renderPass = render_pass;
        iifo.subpass = 0;
        iifo.framebuffer = framebuffers[image_index];
        for (int i = 0; i < 3; i++)
        {
            iifo.subpass = i;
            recorder[i].begin(iifo);
        }

        vkResetCommandBuffer(cmd, 0);
        VkCommandBufferBeginInfo begin_info{};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        vkBeginCommandBuffer(cmd, &begin_info);
        VkClearValue clear_value[7];
        clear_value[0] = {{0.0f, 0.0f, 0.0f, 1.0f}};
        clear_value[1] = {{0.0f, 0.0f, 0.0f, 1.0f}};
        clear_value[2] = {{0.0f, 0.0f, 0.0f, 1.0f}};
        clear_value[3] = {{0.0f, 0.0f, 0.0f, 1.0f}};
        clear_value[4] = {{0.0f, 0.0f, 0.0f, 1.0f}};
        clear_value[5] = {{1.0f, 0.0f}};
        clear_value[6] = {{0.0f, 0.0f, 0.0f, 1.0f}};
        VkRenderPassBeginInfo render_pass_info{};
        render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        render_pass_info.renderPass = render_pass;
        render_pass_info.framebuffer = framebuffers[image_index];
        render_pass_info.renderArea.extent = swapchain.extend_;
        render_pass_info.clearValueCount = 7;
        render_pass_info.pClearValues = clear_value;

        vkCmdBeginRenderPass(cmd, &render_pass_info, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
        recorder[0].wait_than_excute(cmd);

        vkCmdNextSubpass(cmd, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
        recorder[1].wait_than_excute(cmd);

        vkCmdNextSubpass(cmd, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);
        recorder[2].wait_than_excute(cmd);

        vkCmdEndRenderPass(cmd);
        vkEndCommandBuffer(cmd);

        VkPipelineStageFlags wait_stages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        VkSubmitInfo submit_info{};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.waitSemaphoreCount = 1;
        submit_info.pWaitSemaphores = &image_semaphore;
        submit_info.pWaitDstStageMask = wait_stages;
        submit_info.commandBufferCount = 1;
        submit_info.pCommandBuffers = &cmd;
        submit_info.signalSemaphoreCount = 1;
        submit_info.pSignalSemaphores = &submit_semaphore;
        vkQueueSubmit(device.graphics_queue_, 1, &submit_info, frame_fence);

        VkPresentInfoKHR present_info{};
        present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        present_info.waitSemaphoreCount = 1;
        present_info.pWaitSemaphores = &submit_semaphore;
        present_info.swapchainCount = 1;
        present_info.pSwapchains = &swapchain;
        present_info.pImageIndices = &image_index;

        vkQueuePresentKHR(device.present_queue_, &present_info);

        vkDeviceWaitIdle(device);
    }

    tt.destroy(device);
    sampler.destroy(device);

    for (int i = 0; i < 3; i++)
    {
        recorder[i].terminate();
        recorder[i].destroy(device);
    }

    record_th0.join();
    record_th1.join();
    record_th2.join();

    aa.destroy(device);
    camera.destroy(device);

    for (int i = 0; i < framebuffers.size(); i++)
    {
        vkDestroyFramebuffer(device, framebuffers[i], nullptr);
    }
    image_semaphore.destroy(device);
    submit_semaphore.destroy(device);
    frame_fence.destroy(device);
    cmd_pool.destroy(device);
    for (int i = 0; i < 3; i++)
    {
        pipelines[i].destroy(device);
        verts[i].destroy();
        frags[i].destroy();
        pipeline_layouts[i].destroy(device);
        layouts[i].destroy(device);
    }
    vkDestroyRenderPass(device, render_pass, nullptr);
    as::destroy_image_attachments(device, attachments);
    swapchain.destroy();
    device.destroy();
    window.destroy(instance);
    return EXIT_SUCCESS;
}