#define STB_IMAGE_IMPLEMENTATION
#include "engine/texture.hpp"

AS_SCRIPT as::Texture* write(as::Texture::CreateInfo* create_info)
{
    int w, h, chan;
    stbi_uc* pixels = stbi_load(create_info->file_name_.c_str(), &w, &h, &chan, STBI_rgb_alpha);
    if (pixels == nullptr)
    {
        as::Log::error("Do not load image");
        return nullptr;
    }

    as::Texture* tex = new as::Texture;
    tex->format_.width = w;
    tex->format_.height = h;
    tex->format_.depth = chan;

    vk::BufferCreateInfo buffer_info{};
    buffer_info.size = w * h * chan;
    buffer_info.usage = vk::BufferUsageFlagBits::eTransferSrc;
    vma::AllocationCreateInfo alloc_info{};
    alloc_info.usage = vma::MemoryUsage::eAutoPreferHost;
    alloc_info.preferredFlags = vk::MemoryPropertyFlagBits::eHostCoherent;
    alloc_info.flags = vma::AllocationCreateFlagBits::eHostAccessSequentialWrite;
    as::Buffer* stage_buffer = new as::Buffer(buffer_info, alloc_info);
    memcpy(stage_buffer->map_memory(), pixels, buffer_info.size);
    stbi_image_free(pixels);

    vk::ImageCreateInfo image_info{};
    image_info.imageType = vk::ImageType::e2D;
    image_info.extent = tex->format_;
    image_info.extent.depth = 1;
    image_info.mipLevels = 1;
    image_info.arrayLayers = 1;
    image_info.format = vk::Format::eR8G8B8A8Srgb;
    image_info.usage = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled;
    image_info.samples = vk::SampleCountFlagBits::e1;
    alloc_info.usage = vma::MemoryUsage::eAutoPreferDevice;
    alloc_info.preferredFlags = {};
    alloc_info.flags = {};
    tex->image_ = new as::Image(image_info, alloc_info);

    vk::ImageMemoryBarrier barrier{};
    barrier.newLayout = vk::ImageLayout::eTransferDstOptimal;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = *tex->image_;
    barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.srcAccessMask = {};
    barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

    vk::BufferImageCopy region{};
    region.imageSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageExtent = vk::Extent3D(tex->format_.width, tex->format_.height);

    as::CmdBuffer* cmd = create_info->cmd_pool_->alloc_buffer();
    as::begin_cmd(cmd, vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
    cmd->pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eTransfer, //
                         {}, {}, {}, barrier);
    barrier.oldLayout = vk::ImageLayout::eTransferDstOptimal;
    barrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
    barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
    barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
    cmd->pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader, //
                         {}, {}, {}, barrier);
    cmd->end();
    create_info->cmd_pool_->submit_onetime(cmd);
    delete cmd;

    vk::ImageViewCreateInfo view_info{};
    view_info.viewType = vk::ImageViewType::e2D;
    view_info.format = vk::Format::eR8G8B8A8Srgb;
    view_info.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
    view_info.subresourceRange.baseMipLevel = 0;
    view_info.subresourceRange.levelCount = 1;
    view_info.subresourceRange.baseArrayLayer = 0;
    view_info.subresourceRange.layerCount = 1;
    tex->image_->create_image_view(view_info);

    return tex;
}