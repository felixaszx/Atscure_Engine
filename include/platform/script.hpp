#ifndef SCRIPT_DEFINES_HPP
#define SCRIPT_DEFINES_HPP

#include <iostream>
#include <string>
#include <entt/entt.hpp>
#include "api/logging.hpp"
#include "platform/loader.hpp"

#define AS_SCRIPT extern "C" __declspec(dllexport)

namespace as
{
    using init_func = void (*)();
    using finish_func = void (*)();
    using write_func = void (*)(void*);
    using read_func = void* (*)();

    using start_func = void (*)(entt::registry&, const entt::entity);
    using end_func = void (*)(entt::registry&, const entt::entity);
    using update_func = void (*)(entt::registry&, const entt::entity);
    using fixed_func = void (*)(entt::registry&, const entt::entity);

    struct Script
    {
        init_func init{};
        finish_func finish{};
        write_func write{};
        read_func read{};

        start_func start{};
        end_func end{};
        update_func update{};
        fixed_func fixed{};

        Script(DynamicLoader& loader)
        {
            init = LoadFunc(loader, "init");
            fixed = LoadFunc(loader, "fixed");
            write = LoadFunc(loader, "write");
            read = LoadFunc(loader, "read");

            start = LoadFunc(loader, "start");
            end = LoadFunc(loader, "end");
            finish = LoadFunc(loader, "finish");
            update = LoadFunc(loader, "update");
        }
    };

}; // namespace as

#endif // SCRIPT_DEFINES_HPP
