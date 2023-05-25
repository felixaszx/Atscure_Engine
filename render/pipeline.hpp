#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include "api/wrapper.hpp"

namespace as
{
    struct PipelineBuilder
    {
        UniqueObj<ShaderModule> vert_shader_{nullptr};
        UniqueObj<ShaderModule> geom_shader_{nullptr};
        UniqueObj<ShaderModule> frag_shader_{nullptr};
    };
}; // namespace as

#endif // PIPELINE_HPP
