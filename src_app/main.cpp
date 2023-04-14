#include <iostream>
#include <string>
#include "api/api_wrapper.hpp"
#include "platform/platform_wrapper.hpp"

int main(int argc, char** argv)
{
    as::DynamicLoader dsada("build/libtest.dll");
    auto ddd = dsada.get_function<as::Script::Creation>(AS_SCRIPT_CREATION_NAME);
    auto dddd = ddd();
    dddd->init_call();
    delete dddd;

    return EXIT_SUCCESS;
}