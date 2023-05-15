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

    as::MeshGroup mg;
    mg.add_mesh(0,0,0);

    while (render.running())
    {
        as::DefaultRenderer::ResultInfo ss;
        render.poll_events();
        render.get_renderer(0)->render_scene(ss, {}, {});
    }

    return EXIT_SUCCESS;
}