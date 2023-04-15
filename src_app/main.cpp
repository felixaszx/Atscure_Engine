#include <iostream>
#include <string>
#include "api/api_wrapper.hpp"
#include "platform/platform_wrapper.hpp"
#include "engine/engine_wrapper.hpp"

struct Data
{
    int a;
};
Data data_blcok{};

int main(int argc, char** argv)
{
    as::DynamicLoader test_dll("script/bin/libtest.dll");
    as::Script2 ss(test_dll);
    data_blcok.a = 10;
    ss.write(&data_blcok);
    ss.init();

    return EXIT_SUCCESS;
}