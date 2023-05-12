#include <iostream>
#include <string>
#include "vk_api/wrapper.hpp"
#include "as/wrapper.hpp"
#include "module/module.hpp"
#include "exts/exts.hpp"

int main(int argc, char** argv)
{
    as::RenderModule render;
    as::GameModule game;
    as::PhysicModule physic;

    as::UniqueObj<as::DefaultRenderer> d_renderer(render);
    render.add_renderer(d_renderer.get());

    while (render.running())
    {
        render.poll_events();
    }

    return EXIT_SUCCESS;
}