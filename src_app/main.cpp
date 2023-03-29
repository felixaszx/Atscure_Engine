#include <iostream>
#include <string>

#define VMA_IMPLEMENTATION
#define VMA_STATIC_VULKAN_FUNCTIONS  0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 1
#include "at_window.hpp"
#include "at_device.hpp"
#include "at_swapchain.hpp"
#include "at_shader.hpp"
#include "at_image.hpp"
#include "at_pipeline.hpp"
#include "at_mesh.hpp"

int main(int argc, char** argv)
{
    ats::Instance instance{};
    ats::WindowManager window(1920, 1080);
    window.create("test app", instance, true);

    ats::Device device(instance, instance);
    device.create(instance, instance.VALIDATION_LAYERS);

    ats::Swapchain swapchain;
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
    std::vector<ats::ImageAttachment> attachments = ats::create_image_attachments(device,           //
                                                                                  formats, extends, //
                                                                                  samples, usages,  //
                                                                                  aspects);

    VkAttachmentDescription attachment_descriptions[7]{};
    for (uint32_t i = 0; i < 7; i++)
    {
        attachment_descriptions[i].samples = VK_SAMPLE_COUNT_1_BIT;
        attachment_descriptions[i].format = VK_FORMAT_R32G32B32A32_SFLOAT;
        attachment_descriptions[i].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        attachment_descriptions[i].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        attachment_descriptions[i].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachment_descriptions[i].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachment_descriptions[i].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachment_descriptions[i].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    }
    attachment_descriptions[5].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
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

    ats::DescriptorLayout layouts[3]{};
    layouts[0].add_binding(0, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);
    for (int i = 0; i < 4; i++)
    {
        layouts[1].add_binding(i, 1, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, VK_SHADER_STAGE_FRAGMENT_BIT);
    }
    layouts[2].add_binding(0, 1, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, VK_SHADER_STAGE_FRAGMENT_BIT);
    for (int i = 0; i < 3; i++)
    {
        layouts[i].create(device);
    }
    ats::PipelineLayout pipeline_layouts[3]{};
    for (int i = 0; i < 3; i++)
    {
        pipeline_layouts[i].add_layout(layouts[i]);
        if (i == 1)
        {
            pipeline_layouts[i].add_constant(0, 80, VK_SHADER_STAGE_FRAGMENT_BIT);
        }
        pipeline_layouts[i].create(device);
    }

    auto binding_description = ats::Mesh::get_bindings();
    auto attribute_description = ats::Mesh::get_attributes();
    ats::ShaderModule verts[3]{};
    ats::ShaderModule frags[3]{};
    for (int i = 0; i < 3; i++)
    {
        verts[i].create(device, ats::ShaderModule::read_file("res/shader/vert" + std::to_string(i) + ".spv"), "main",
                        VK_SHADER_STAGE_VERTEX_BIT);
        frags[i].create(device, ats::ShaderModule::read_file("res/shader/frag" + std::to_string(i) + ".spv"), "main",
                        VK_SHADER_STAGE_FRAGMENT_BIT);
    }

    ats::GraphicPipeline pipelines[3]{};
    for (int i = 0; i < 3; i++)
    {
        pipelines[i].stages_.set_vert_shader(verts[i].stage_info_);
        pipelines[i].stages_.set_frag_shader(frags[i].stage_info_);
    }

    VkPipelineColorBlendAttachmentState color_blend_attachment0{};
    color_blend_attachment0.colorWriteMask = ats::ColorWrite::RGBA;
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
    color_blend_attachment1.colorWriteMask = ats::ColorWrite::RGBA;
    pipelines[1].set_view_port_state(1, 1);
    pipelines[1].set_rasterizer(VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE);
    pipelines[1].add_color_blending(color_blend_attachment1);
    pipelines[1].set_depth_stencil(false, false);
    pipelines[1].create(device, pipeline_layouts[1], render_pass, 1);

    VkPipelineColorBlendAttachmentState color_blend_attachment2{};
    color_blend_attachment2.colorWriteMask = ats::ColorWrite::RGBA;
    pipelines[2].set_view_port_state(1, 1);
    pipelines[2].set_rasterizer(VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE);
    pipelines[2].add_color_blending(color_blend_attachment2);
    pipelines[2].set_depth_stencil(false, false);
    pipelines[2].create(device, pipeline_layouts[2], render_pass, 2);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }

    for (int i = 0; i < 3; i++)
    {
        pipelines[i].destroy(device);
        verts[i].destroy();
        frags[i].destroy();
        pipeline_layouts[i].destroy(device);
        layouts[i].destroy(device);
    }
    vkDestroyRenderPass(device, render_pass, nullptr);
    ats::destroy_image_attachments(device, attachments);
    swapchain.destroy();
    device.destroy();
    window.destroy(instance);
    return EXIT_SUCCESS;
}