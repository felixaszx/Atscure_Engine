#include <iostream>
#include <string>
#include "api/api_wrapper.hpp"
#include "platform/platform_wrapper.hpp"
#include "module/module_wrapper.hpp"
#include "as/as_wrapper.hpp"

#include <ode/ode.h>

int main(int argc, char** argv)
{
    as::DeviceI devicei{};
    as::DynamicLoader base_dll("module/bin/m_base.dll");
    as::ModuleSingleton<as::BaseModuleSingleton> base(base_dll, {false, &devicei});

    as::DynamicLoader renderer_dll("module/bin/m_renderer.dll");
    as::ModuleSingleton<as::RendererModuleSingleton> renderer(renderer_dll, base);

    as::DynamicLoader game_dll("module/bin/m_game.dll");
    as::ModuleSingleton<as::GameModuleSingleton> game(game_dll, {&base, &devicei});

    as::CpuTimer timer;

    float delta_s = 0;
    float delta_ms = 0;

    as::Scene* scene = game.load_scene(0);
    while (!glfwWindowShouldClose(base.window_->window_))
    {
        timer.start();
        devicei.prev_mouse_ = devicei.curr_mouse_;
        glfwPollEvents();
        devicei.delta_mouse_ = {devicei.curr_mouse_.x_ - devicei.prev_mouse_.x_, //
                                devicei.curr_mouse_.y_ - devicei.prev_mouse_.y_};
        devicei.frame_time_ = delta_ms;

        scene->update(delta_s);
        renderer.render_scene(scene, base.swapchian_->acquire_next_image(UINT64_MAX, renderer.image_sem_));
        base.swapchian_->present({renderer.submit_sem_});

        base.device_->waitIdle();

        timer.finish();
        delta_s = timer.get_duration_second();
        delta_ms = timer.get_duration_ms();
    }
    scene->finish();

    return EXIT_SUCCESS;
}