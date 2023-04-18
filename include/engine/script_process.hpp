#ifndef SCRIPT_PROCESS_HPP
#define SCRIPT_PROCESS_HPP

#include <map>
#include "../api/api_wrapper.hpp"
#include "../platform/platform_wrapper.hpp"
#include "scene.hpp"

namespace as
{
    struct ScriptProcessor
    {
        struct CallFunc
        {
            Entity* entity_;
            FUNC_SIG func;
        };

        std::array<std::vector<CallFunc>, AS_SCRIPT_MAX_ORDER> script_maps_[Script::FUNC_COUNTS]{};

        AS_SCRIPT_MEM_FUNC void check_scene(Scene& scene);
        AS_SCRIPT_MEM_FUNC void call_func(Script::Func func_enum);
    };
}; // namespace as

#endif // SCRIPT_PROCESS_HPP
