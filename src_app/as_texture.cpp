#define STB_IMAGE_IMPLEMENTATION
#include "as_texture.hpp"

namespace ats
{
    Texture create_image_texture(Device device, VkCommandPool cmd_pool, const std::string& file_name)
    {
        int w, h, chan;

        stbi_uc* pixels = stbi_load(file_name.c_str(), &w, &h, &chan, STBI_rgb_alpha);
        if (pixels == nullptr)
        {
            Log::error("Do not load image");
            return Texture({}, nullptr);
        }

        Texture tmp_tex({casts(uint32_t, w), casts(uint32_t, h), casts(uint32_t, chan)}, pixels);
        tmp_tex.create(device);

        return tmp_tex;
    }

    Texture::Texture(VkExtent3D format, stbi_uc* pixels)
        : stage_buffer_(VMA_MEMORY_USAGE_AUTO_PREFER_HOST, //
                        VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT),
          pixels_(pixels),
          format_(format),
          size_(format.width * format.height * format.depth)
    {
    }

    Texture::operator stbi_uc*()
    {
        return pixels_;
    }

    void Texture::create(Device device)
    {
        VkImageCreateInfo image_info{};
        image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        image_info.imageType = VK_IMAGE_TYPE_2D;
        image_info.extent.width = format_.width;
        image_info.extent.height = format_.height;
        image_info.extent.depth = 1;
        image_info.mipLevels = 1;
        image_info.arrayLayers = 1;
        image_info.format = VK_FORMAT_R8G8B8A8_SRGB;
        image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
        image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        image_info.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        image_info.samples = VK_SAMPLE_COUNT_1_BIT;
        VmaAllocationCreateInfo alloc_info{};
        alloc_info.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;
        VmaAllocationInfo alloc_result{};
        vmaCreateImage(device, &image_info, &alloc_info, this->ptr(), this->ptr(), &alloc_result);
        this->set(alloc_result.deviceMemory);

        void* staging_map_ = stage_buffer_.create_mapped(device, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, //
                                                         VK_SHARING_MODE_EXCLUSIVE, size_);
        memcpy(staging_map_, pixels_, size_);
        pixels_ = nullptr;
    }

    void Texture::load(Device device, VkCommandPool pool)
    {
        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = *this;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        VkBufferImageCopy region{};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;
        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 0;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;
        region.imageOffset = {0, 0, 0};
        region.imageExtent = {format_.width, format_.height, 1};

        SingleTimeCmd cmd;
        cmd.begin(device, pool);
        vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, //
                             0, 0, nullptr, 0, nullptr, 1, &barrier);
        vkCmdCopyBufferToImage(cmd, stage_buffer_, *this, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, //
                             0, 0, nullptr, 0, nullptr, 1, &barrier);
        cmd.end(device, pool);
    }

    void Texture::create_image_view(VkDevice device)
    {
        VkImageViewCreateInfo view_info{};
        view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        view_info.image = *this;
        view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
        view_info.format = VK_FORMAT_R8G8B8A8_SRGB;
        view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        view_info.subresourceRange.baseMipLevel = 0;
        view_info.subresourceRange.levelCount = 1;
        view_info.subresourceRange.baseArrayLayer = 0;
        view_info.subresourceRange.layerCount = 1;
        vkCreateImageView(device, &view_info, nullptr, this->ptr());
    }

    void Texture::destroy(Device device)
    {
        stage_buffer_.destroy(device);
        vkDestroyImageView(device, *this, nullptr);
        vmaDestroyImage(device, *this, *this);
    }
}; // namespace ats