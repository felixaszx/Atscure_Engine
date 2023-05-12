#define STB_IMAGE_IMPLEMENTATION
#include "as/texture.hpp"

as::Texture::Texture(const CreateInfo& create_info)
{
    stbi_set_flip_vertically_on_load(true);
    int w, h, chan;
    stbi_uc* pixels = stbi_load(create_info.file_name_.c_str(), &w, &h, &chan, STBI_rgb_alpha);
    if (pixels == nullptr)
    {
        as::Log::error("Do not load image");
    }

    chan = 4;
    format_.width = w;
    format_.height = h;
    format_.depth = chan;
    mip_levels_ = create_info.disable_mip_ ? 1 : std::floor(std::log2(std::max(w, h))) + 1;

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
    image_info.extent = format_;
    image_info.extent.depth = 1;
    image_info.mipLevels = mip_levels_;
    image_info.arrayLayers = 1;
    image_info.format = vk::Format::eR8G8B8A8Srgb;
    image_info.usage = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eTransferSrc //
                       | vk::ImageUsageFlagBits::eSampled;
    image_info.samples = vk::SampleCountFlagBits::e1;
    alloc_info.usage = vma::MemoryUsage::eAutoPreferDevice;
    alloc_info.preferredFlags = {};
    alloc_info.flags = {};
    image_.reset(new as::Image(image_info, alloc_info));

    vk::ImageMemoryBarrier barrier{};
    barrier.newLayout = vk::ImageLayout::eTransferDstOptimal;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = *image_;
    barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = mip_levels_;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.srcAccessMask = {};
    barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;

    vk::BufferImageCopy region{};
    region.imageSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageExtent = vk::Extent3D(format_.width, format_.height, 1);

    as::CmdBuffer* cmd = create_info.cmd_pool_->alloc_buffer();
    as::begin_cmd(cmd, vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
    cmd->pipelineBarrier(vk::PipelineStageFlagBits::eTopOfPipe, vk::PipelineStageFlagBits::eTransfer, //
                         {}, {}, {}, barrier);
    cmd->copyBufferToImage(*stage_buffer, *image_, vk::ImageLayout::eTransferDstOptimal, region);

    int32_t mip_w = format_.width;
    int32_t mip_h = format_.height;
    for (int i = 1; i < mip_levels_; i++)
    {
        barrier.subresourceRange.levelCount = 1;
        barrier.oldLayout = vk::ImageLayout::eTransferDstOptimal;
        barrier.newLayout = vk::ImageLayout::eTransferSrcOptimal;
        barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
        barrier.dstAccessMask = vk::AccessFlagBits::eTransferRead;
        barrier.subresourceRange.baseMipLevel = i - 1;
        cmd->pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eTransfer, //
                             {}, {}, {}, barrier);

        vk::ImageBlit blit{};
        blit.srcOffsets[1] = vk::Offset3D(mip_w, mip_h, 1);
        blit.srcSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
        blit.srcSubresource.mipLevel = i - 1;
        blit.srcSubresource.baseArrayLayer = 0;
        blit.srcSubresource.layerCount = 1;
        blit.dstOffsets[1] = vk::Offset3D(mip_w > 1 ? mip_w / 2 : 1, //
                                          mip_h > 1 ? mip_h / 2 : 1, //
                                          1);
        blit.dstSubresource.aspectMask = vk::ImageAspectFlagBits::eColor;
        blit.dstSubresource.mipLevel = i;
        blit.dstSubresource.baseArrayLayer = 0;
        blit.dstSubresource.layerCount = 1;
        cmd->blitImage(*image_, vk::ImageLayout::eTransferSrcOptimal, //
                       *image_, vk::ImageLayout::eTransferDstOptimal, //
                       blit, vk::Filter::eLinear);

        barrier.oldLayout = vk::ImageLayout::eTransferSrcOptimal;
        barrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
        barrier.srcAccessMask = vk::AccessFlagBits::eTransferRead;
        barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
        barrier.subresourceRange.baseMipLevel = i - 1;
        cmd->pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader, //
                             {}, {}, {}, barrier);

        mip_w > 1 ? mip_w /= 2 : mip_w;
        mip_h > 1 ? mip_h /= 2 : mip_h;
    }
    barrier.oldLayout = vk::ImageLayout::eTransferDstOptimal;
    barrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
    barrier.srcAccessMask = vk::AccessFlagBits::eTransferRead;
    barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
    barrier.subresourceRange.baseMipLevel = mip_levels_ - 1;
    cmd->pipelineBarrier(vk::PipelineStageFlagBits::eTransfer, vk::PipelineStageFlagBits::eFragmentShader, //
                         {}, {}, {}, barrier);

    cmd->end();
    create_info.cmd_pool_->submit_onetime(cmd);
    ffree(cmd);
    ffree(stage_buffer);

    vk::ImageViewCreateInfo view_info{};
    view_info.viewType = vk::ImageViewType::e2D;
    view_info.format = vk::Format::eR8G8B8A8Srgb;
    view_info.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
    view_info.subresourceRange.baseMipLevel = 0;
    view_info.subresourceRange.levelCount = mip_levels_;
    view_info.subresourceRange.baseArrayLayer = 0;
    view_info.subresourceRange.layerCount = 1;
    image_->create_image_view(view_info);

    des_info_.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
    des_info_.imageView = *image_;
    des_info_.sampler = create_info.sampler_;
}