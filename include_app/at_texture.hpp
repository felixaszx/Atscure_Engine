#ifndef AT_TEXTURE_HPP
#define AT_TEXTURE_HPP

#include <stb/stb_image.h>

#include "at_image.hpp"
#include "at_buffer.hpp"

namespace ats
{
    class Texture : public ImageBase
    {
      private:
        stbi_uc* pixels_ = nullptr;
        Buffer stage_buffer_;

      public:
        bool usable = false;
        VkExtent3D format_{};
        VkDeviceSize size_ = 0;

        Texture();
        Texture(VkExtent3D format, stbi_uc* pixels);
        operator stbi_uc*();

        void create(Device device);
        void load(Device device, VkCommandPool pool);
        void create_image_view(VkDevice device);

        void destroy(Device device);
    };

    Texture create_image_texture(Device device, VkCommandPool cmd_pool, const std::string& file_name);

}; // namespace ats

#endif // AT_TEXTURE_HPP
