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
            std::string file_name_ = "";
            as::CmdPool* cmd_pool_ = nullptr;
        };

        vk::Extent3D format_{};
        as::Image* image_{};
    };

}; // namespace as

#endif // TEXTURE_HPP
