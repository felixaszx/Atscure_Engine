#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <stb/stb_image.h>
#include "vk_api/wrapper.hpp"
#include "utils.hpp"

namespace as
{
    struct Texture
    {
      private:
        uint32_t mip_levels_ = 1;
        vk::Extent3D format_{};
        UniqueObj<as::Image> image_{nullptr};

      public:
        struct CreateInfo
        {
            bool disable_mip_ = false;
            std::string file_name_ = "";
            as::CmdPool* cmd_pool_ = nullptr;
            vk::Sampler sampler_{};
        };

        vk::DescriptorImageInfo des_info_{};

        Texture(const CreateInfo& create_info);
    };
}; // namespace as

#endif // TEXTURE_HPP
