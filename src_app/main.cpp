#include <iostream>
#include <string>
#include "api/api_wrapper.hpp"
#include "platform/platform_wrapper.hpp"
#include "engine/engine_wrapper.hpp"

int main(int argc, char** argv)
{
    as::DynamicLoader engine_dll("script/bin/libengine.dll");
    as::DynamicLoader renderer_dll("script/bin/librenderer.dll");
    as::Script engine_class(engine_dll);
    as::Script renderer_class(renderer_dll);

    as::Engine* engine = engine_class.create<as::Engine>();
    as::Renderer* renderer = renderer_class.create<as::Renderer>(engine);

    while (!glfwWindowShouldClose(engine->window_->window_))
    {
        glfwPollEvents();

        renderer->wait_idle();
    }

    delete renderer;
    delete engine;

    return EXIT_SUCCESS;
}