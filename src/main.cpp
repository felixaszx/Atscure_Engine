#include <iostream>
#include "render/mesh.hpp"

int main(int argc, char** argv)
{
    as::Window w(1920, 1080);
    as::Context c(true);
    w.create_surface(c);

    as::Device d(c, c.VALIDATION_LAYERS);
    as::Swapchain s(w, c, d);

    as::MeshData md(10);
    std::vector<as::Mesh> m;
    as::MeshDataLoader mdl("res/model/sponza/sponza.obj", md, m);

    while (!glfwWindowShouldClose(w.window_))
    {
        glfwPollEvents();
    }
    return EXIT_SUCCESS;
}