#include <iostream>
#include <string>
#include "api/api_wrapper.hpp"
#include "platform/platform_wrapper.hpp"
#include "engine/engine_wrapper.hpp"

int main(int argc, char** argv)
{
    as::DynamicLoader renderer_dll("script/bin/librenderer.dll");
    std::unique_ptr<as::Renderer> renderer((as::Renderer*)as::Script::load_creation(renderer_dll));
    renderer->init();
    renderer->finish();

    return EXIT_SUCCESS;
}