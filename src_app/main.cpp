#include <iostream>
#include <string>
#include "api/api_wrapper.hpp"
#include "platform/platform_wrapper.hpp"
#include "module/module_wrapper.hpp"

int main(int argc, char** argv)
{
    as::DynamicLoader base_dll("module/bin/m_base.dll");
    as::ModuleSingleton<as::BaseModuleSingleton> base(base_dll, true);

    return EXIT_SUCCESS;
}