#ifndef SCRIPT_DEFINES_HPP
#define SCRIPT_DEFINES_HPP

#include <iostream>
#include <string>
#include "api/logging.hpp"
#include "platform/loader.hpp"

#define AS_SCRIPT               extern "C" __declspec(dllexport)
#define AS_SCRIPT_CREATION_NAME "script_obj_creation"
#define AS_SCRIPT_CREATE_FUNC(type, script) \
    AS_SCRIPT script* script_obj_creation() \
    {                                       \
        return new type();                  \
    }

namespace as
{
    // all script must inhritance from this
    struct ScriptGeneral
    {
        // each script must have a object creation function with this signature
        // and name: <AS_SCRIPT_CREATION script_obj_creation()>
        using Creation = ScriptGeneral* (*)();
        virtual ~ScriptGeneral(){};

        virtual void init() = 0;
        virtual void finish() = 0;

        inline static ScriptGeneral* create(DynamicLoader& loader)
        {
            return loader.get_function<Creation>(AS_SCRIPT_CREATION_NAME)();
        }
    };

    using init_func = void (*)();
    using finish_func = void (*)();
    using update_func = void (*)();
    using fixed_func = void (*)();
    using write_func = void (*)(void*);
    using read_func = void* (*)();

    struct Script2
    {
        init_func init{};
        finish_func finish{};
        update_func update{};
        fixed_func fixed{};
        write_func write{};
        read_func read{};

        Script2(DynamicLoader& loader)
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
