#ifndef AT_MATERIAL_HPP
#define AT_MATERIAL_HPP

#include "at_texture.hpp"

namespace ats
{
    struct Material
    {
        std::string name_;
        Texture albedo;
        Texture specular;
        Texture ambient;
        Texture transparency;
    };

}; // namespace ats

#endif // AT_MATERIAL_HPP
