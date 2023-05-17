#include <iostream>
#include "render/render.hpp"

int main(int argc, char** argv)
{
    as::RenderContext render_context(1920, 1080);

    as::MeshDataGroup group(10);
    group.add_buffers({{}, {}, {}, {}}, {1, 2, 3, 4}, render_context.utils_pool());

    as::DefferedZero d(render_context.vk_swapchain_);
    d.update_ubo({}, {});

    while (render_context.running())
    {
        render_context.update_window();
    }

    return EXIT_SUCCESS;
}