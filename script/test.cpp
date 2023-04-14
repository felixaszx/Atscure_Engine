#include <iostream>
#include "api/logging.hpp"
#include "platform/script_defines.hpp"

struct TestScriptOne : public as::ScriptGeneral
{
    void init_call() override { std::cout << "Calliing test's init_call()"; }
};

AS_SCRIPT_CREATION_FUNC_SIG
{
    return new TestScriptOne();
}