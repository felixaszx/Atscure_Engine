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

    as::MeshLoader loader("res/model/sponza/sponza.obj");
    as::MeshGroup mg(loader.vertices_, loader.indices_, render.utils_pool_);
    for (uint32_t i = 0; i < loader.mesh_count_; i++)
    {
        mg.add_mesh(loader.vert_buffer_offsets_[i],  //
                    loader.index_buffer_offsets_[i], //
                    loader.mesh_indices_count_[i]);
    }

    while (render.running())
    {
        as::DefaultRenderer::ResultInfo ss;
        render.poll_events();
        render.get_renderer(0)->render_scene(ss, {}, {});
    }

    return EXIT_SUCCESS;
}