#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <stb/stb_image.h>
#include "../api/api_wrapper.hpp"
#include "../platform/script.hpp"

namespace as
{
    struct Texture
    {
        struct CreateInfo
        {
            bool disable_mip_ = false;
            std::string file_name_ = "";
            as::CmdPool* cmd_pool_ = nullptr;
            vk::Sampler sampler_{};
        };

        uint32_t mip_levels_ = 1;
        vk::DescriptorImageInfo des_info_{};
        vk::Extent3D format_{};
        as::Image* image_{};

        AS_SCRIPT_MEM_FUNC ~Texture();
    };

}; // namespace as

#endif // TEXTURE_HPP
