#ifndef SCRIPT_DEFINES_HPP
#define SCRIPT_DEFINES_HPP

#include <iostream>
#include <string>
#include "api/logging.hpp"
#include "platform/loader.hpp"

#define AS_SCRIPT extern "C" __declspec(dllexport)

namespace as
{
    using init_func = void (*)();
    using finish_func = void (*)();
    using update_func = void (*)();
    using fixed_func = void (*)();
    using write_func = void (*)(void*);
    using read_func = void* (*)();

    struct Script
    {
        init_func init{};
        finish_func finish{};
        update_func update{};
        fixed_func fixed{};
        write_func write{};
        read_func read{};

        Script(DynamicLoader& loader)
        {
            init = LoadFunc(loader, "init");
            finish = LoadFunc(loader, "finish");
            update = LoadFunc(loader, "update");
            fixed = LoadFunc(loader, "fixed");
            write = LoadFunc(loader, "write");
            read = LoadFunc(loader, "read");
        }
    };

}; // namespace as

#endif // SCRIPT_DEFINES_HPP
