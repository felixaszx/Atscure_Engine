#include <iostream>
#include <string>
#include "vk_api/wrapper.hpp"
#include "as/wrapper.hpp"
#include "module/module.hpp"

int main(int argc, char** argv)
{
    as::RenderModule render;
    as::GameModule game;
    as::PhysicModule physic;

    while (render.running())
    {
        render.poll_events();
    }

    return EXIT_SUCCESS;
}