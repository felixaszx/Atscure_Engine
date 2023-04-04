#ifndef AT_TEXTURE_HPP
#define AT_TEXTURE_HPP

#include <stb/stb_image.h>

#include "as_image.hpp"
#include "as_buffer.hpp"

namespace as
{
    class Texture : public ImageBase
    {
      private:
        stbi_uc* pixels_ = nullptr;
        Buffer stage_buffer_;

      public:
        const VkExtent3D format_{};
        const VkDeviceSize size_ = 0;

        Texture(VkExtent3D format, stbi_uc* pixels);
        operator stbi_uc*();

        void create(Device device);
        void load(Device device, VkCommandPool pool);
        void create_image_view(VkDevice device);

        void destroy(Device device);
    };

    Texture create_image_texture(Device device, VkCommandPool cmd_pool, const std::string& file_name);

}; // namespace as

#endif // AT_TEXTURE_HPP
