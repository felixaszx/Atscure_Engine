#include <iostream>
#include <string>
#include "api/api_wrapper.hpp"
#include "platform/platform_wrapper.hpp"
#include "engine/engine_wrapper.hpp"

int main(int argc, char** argv)
{
    as::DynamicLoader engine_dll("script/bin/libengine.dll");
    as::Script engine_script(engine_dll);
    as::Engine* engine = (as::Engine*)engine_script.read();
    engine_script.init();

    as::DynamicLoader renderer_dll("script/bin/librenderer.dll");
    as::Script renderer_script(renderer_dll);
    as::Renderer* renderer = as::ScriptCreate(renderer_script, engine);

    renderer->render_scene({});

    delete renderer;
    engine_script.finish();
    return EXIT_SUCCESS;
}