#ifndef SCRIPT_DEFINES_HPP
#define SCRIPT_DEFINES_HPP

#include <iostream>
#include <string>
#include <entt/entt.hpp>
#include "api/logging.hpp"
#include "platform/loader.hpp"

#define AS_SCRIPT          extern "C" __declspec(dllexport)
#define AS_SCRIPT_MEM_FUNC virtual
#define AS_GAME_SCRIPT(this_entity_name)     \
    as::Entity this_entity_name;             \
    AS_SCRIPT void write(as::Entity* entity) \
    {                                        \
        this_entity_name = *entity;          \
    }                                        \
    AS_SCRIPT void* read()                   \
    {                                        \
        return nullptr;                      \
    }

namespace as
{
    using init_func = void (*)();
    using finish_func = void (*)();
    using write_func = void (*)(const void*);
    using read_func = void* (*)();

    using start_func = void (*)();
    using end_func = void (*)();
    using update_func = void (*)();
    using fixed_func = void (*)();

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

        template <typename T, typename C = typename T::CreateInfo>
        T* create(C* create_info)
        {
            write(create_info);
            return (T*)read();
        }

        template <typename T, typename C = typename T::CreateInfo>
        T* create(const C& create_info)
        {
            write(&create_info);
            return (T*)read();
        }

        template <typename T>
        T* create()
        {
            return (T*)read();
        }
    };

}; // namespace as

#endif // SCRIPT_DEFINES_HPP
