#include <iostream>
#include <string>
#include "vk_api/wrapper.hpp"
#include "as/wrapper.hpp"

struct ss
{
    ss(int s) {}
    ~ss() { std::cout << 11; }
};

int main(int argc, char** argv)
{
    as::UniqueObj<int> ss = 4;
    as::VirtualObj<int> ss2 = ss;
    std::cout << *ss2;

    return EXIT_SUCCESS;
}