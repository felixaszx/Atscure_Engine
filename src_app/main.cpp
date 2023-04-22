#include <iostream>
#include <string>
#include "api/api_wrapper.hpp"
#include "platform/platform_wrapper.hpp"
#include "module/module_wrapper.hpp"
#include "as/as_wrapper.hpp"

int main(int argc, char** argv)
{
    as::DeviceI devicei{};
    as::DynamicLoader base_dll("module/bin/m_base.dll");
    as::ModuleSingleton<as::BaseModuleSingleton> base(base_dll, {true, &devicei});

    as::DynamicLoader renderer_dll("module/bin/m_renderer.dll");
    as::ModuleSingleton<as::RendererModuleSingleton> renderer(renderer_dll, base);

    as::DynamicLoader game_dll("module/bin/m_game.dll");
    as::ModuleSingleton<GameModuleSingleton> game(game_dll, {&base, &devicei});

    as::Scene* scene = game.load_scene();

    scene->start();
    while (!glfwWindowShouldClose(base.window_->window_))
    {
        glfwPollEvents();

        scene->update(10);
        renderer.render_scene(scene, base.swapchian_->acquire_next_image(UINT64_MAX, renderer.image_sem_));
        base.swapchian_->present({renderer.submit_sem_});

        base.device_->waitIdle();
    }
    scene->finish();

    return EXIT_SUCCESS;
}