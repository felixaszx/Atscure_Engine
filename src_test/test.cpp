#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include "third_party/glms.hpp"
#include <ode/ode.h>
#include <entt/entt.hpp>

namespace ode
{
    class Ode
    {
        Ode() { dInitODE(); }
        ~Ode() { dCloseODE(); }
    };
}; // namespace ode

int main(int argc, char** argv)
{
    using namespace std::chrono_literals;

    {
    }

    return 0;
}