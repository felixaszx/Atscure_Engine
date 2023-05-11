#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include "third_party/glms.hpp"
#include <ode/ode.h>
#include <entt/entt.hpp>


int main(int argc, char** argv)
{
    using namespace std::chrono_literals;
    dInitODE();

    dBody ss;
    

    dCloseODE();

    return 0;
}