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
    AS_SCRIPT void* read(void* data)         \
    {                                        \
        return nullptr;                      \
    }

namespace as
{
    using FUNC_SIG = void* (*)(const void*);

    struct Script
    {
        inline static const uint32_t FUNC_COUNTS = 8;
        enum Func
        {
            INIT = 0,
            FINISH,
            WRITE,
            READ,

            START,
            END,
            UPDATE,
            FIXED
        };
        inline static const char* FUNC_NAMES[FUNC_COUNTS] = {"init",  "finish", "write",  "read", //
                                                             "start", "end",    "update", "fixed"};
        FUNC_SIG funcs[FUNC_COUNTS]{};

        Script(DynamicLoader& loader);

        template <typename T, typename C = typename T::CreateInfo>
        T* create(C* create_info)
        {
            funcs[WRITE](create_info);
            return (T*)funcs[READ](nullptr);
        }

        template <typename T>
        T* create()
        {
            return (T*)funcs[READ](nullptr);
        }
    };

}; // namespace as

#endif // SCRIPT_DEFINES_HPP
