#include "api/image.hpp"

as::Image::Image(const vk::ImageCreateInfo& image_info, const vma::AllocationCreateInfo& alloc_info)
{
    vma::AllocationInfo finish_info{};
    auto result = device_->allocator_.createImage(image_info, alloc_info, finish_info);
    auto_set(*this, result.first, result.second, finish_info.deviceMemory);
}

as::Image::Image(const vk::ImageCreateInfo& image_info, const vma::AllocationCreateInfo& alloc_info,
                 vk::ImageViewCreateInfo view_info)
    : Image(image_info, alloc_info)
{
    create_image_view(view_info);
}

as::Image::~Image()
{
    if (VkImageView(*this) != VK_NULL_HANDLE)
    {
        device_->destroyImageView(*this);
    }
    device_->allocator_.destroyImage(*this, *this);
}

void as::Image::create_image_view(vk::ImageViewCreateInfo view_info)
{
    view_info.image = *this;
    sset(*this, device_->createImageView(view_info));
}

void as::Image::destroy_image_view()
{
    device_->destroyImageView(*this);
    casts(vk::ImageView&, *this) = VK_NULL_HANDLE;
}

as::SwapchainImage::SwapchainImage(vk::Image& vk_image, vk::Format format)
    : vk::Image(vk_image)
{
    vk::ImageViewCreateInfo create_info{};
    create_info.image = vk_image;
    create_info.viewType = vk::ImageViewType::e2D;
    create_info.format = format;

    create_info.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
    create_info.subresourceRange.baseMipLevel = 0;
    create_info.subresourceRange.levelCount = 1;
    create_info.subresourceRange.baseArrayLayer = 0;
    create_info.subresourceRange.layerCount = 1;

    sset(*this, device_->createImageView(create_info));
}

as::SwapchainImage::~SwapchainImage()
{
    device_->destroyImageView(*this);
}


std::vector<as::Image*> as::create_image_attachments(std::vector<vk::Format> formats,              //
                                                     std::vector<vk::Extent2D> extends,            //
                                                     std::vector<vk::SampleCountFlagBits> samples, //
                                                     std::vector<vk::ImageUsageFlags> usages,      //
                                                     std::vector<vk::ImageAspectFlags> aspects)
{
    std::vector<Image*> attachments(formats.size());
    for (int i = 0; i < attachments.size(); i++)
    {
        vk::ImageCreateInfo create_info{};
        create_info.imageType = vk::ImageType::e2D;
        create_info.extent.width = extends[i].width;
        create_info.extent.height = extends[i].height;
        create_info.extent.depth = 1;
        create_info.mipLevels = 1;
        create_info.arrayLayers = 1;
        create_info.format = formats[i];
        create_info.tiling = vk::ImageTiling::eOptimal;
        create_info.initialLayout = vk::ImageLayout::eUndefined;
        create_info.usage = usages[i];
        create_info.sharingMode = vk::SharingMode::eExclusive;
        create_info.samples = samples[i];
        VmaAllocationCreateInfo alloc_info{};
        alloc_info.usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE;
        VmaAllocationInfo finish_info{};

        vk::ImageViewCreateInfo view_info{};
        view_info.viewType = vk::ImageViewType::e2D;
        view_info.format = formats[i];
        view_info.subresourceRange.aspectMask = aspects[i];
        view_info.subresourceRange.baseMipLevel = 0;
        view_info.subresourceRange.levelCount = 1;
        view_info.subresourceRange.baseArrayLayer = 0;
        view_info.subresourceRange.layerCount = 1;
        attachments[i] = new Image(create_info, alloc_info, view_info);
    }

    return attachments;
}
