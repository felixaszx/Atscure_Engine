#ifndef SCRIPT_DEFINES_HPP
#define SCRIPT_DEFINES_HPP

#include <iostream>
#include <string>
#include "api/logging.hpp"
#include "platform/loader.hpp"

#define AS_SCRIPT_CREATION_NAME "script_obj_creation"
#define AS_SCRIPT_CREATION_FUNC(type)                                  \
    extern "C" __declspec(dllexport) as::Script* script_obj_creation() \
    {                                                                  \
        return new type();                                             \
    }

namespace as
{
    // all script must inhritance from this
    struct Script
    {
        // each script must have a object creation function with this signature
        // and name: <AS_SCRIPT_CREATION script_obj_creation()>
        using Creation = Script* (*)();
        virtual ~Script(){};

        virtual void init_call() { as::Log::info("Calling [Script] init_call()"); };
        virtual void finish_call() { as::Log::info("Calling [Script] done_call()"); };
        virtual void frame_call() { as::Log::info("Calling [Script] frame_call()"); };
        virtual void fixed_call() { as::Log::info("Calling [Script] fixed_call()"); };

        inline static Script* load_creation(DynamicLoader& loader)
        {
            return loader.get_function<Creation>(AS_SCRIPT_CREATION_NAME)();
        }
    };

}; // namespace as

#endif // SCRIPT_DEFINES_HPP
