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

    as::ModuleManager module(render, game, physic);

    return EXIT_SUCCESS;
}