#ifndef SCRIPT_DEFINES_HPP
#define SCRIPT_DEFINES_HPP

#include <iostream>
#include <string>
#include "api/logging.hpp"

#define AS_SCRIPT_CREATION_FUNC_NAME "script_obj_creation"
#define AS_SCRIPT_CREATION_FUNC_SIG  extern "C" __declspec(dllexport) as::ScriptGeneral* script_obj_creation()

namespace as
{
    // all script must inhritance from this
    struct ScriptGeneral
    {
        // each script must have a object creation function with this signature
        // and name: <AS_SCRIPT_CREATION script_obj_creation()>
        using Creation = ScriptGeneral* (*)();

        virtual void init_call() { as::Log::info("Calling [ScriptGeneral] init_call()"); };
        virtual void test_call() { as::Log::info("Calling [ScriptGeneral] testcall()"); };
        virtual void done_call() { as::Log::info("Calling [ScriptGeneral] done_call()"); };
        virtual void frame_call() { as::Log::info("Calling [ScriptGeneral] frame_call()"); };
        virtual void fixed_call() { as::Log::info("Calling [ScriptGeneral] fixed_call()"); };
    };

}; // namespace as

#endif // SCRIPT_DEFINES_HPP
