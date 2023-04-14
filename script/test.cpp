#include <iostream>
#include "api/logging.hpp"
#include "platform/script.hpp"

struct TestScriptOne : public as::ScriptGeneral
{
    void init_call() override { std::cout << "Calliing test's init_call()" << std::endl; }
};

AS_SCRIPT_CREATION_SIG
{
    return new TestScriptOne();
}