#include "api/context.hpp"
#include "api/device.hpp"

int main(int argc, char** argv)
{
    auto window = std::make_unique<as::Window>(1920, 1080);
    auto context = std::make_unique<as::Context>(true);
    window->create_surface(*context);

    auto device = std::make_unique<as::Device>(*context, context->VALIDATION_LAYERS);

    delete device.release();
    delete context.release();
    delete window.release();

    return EXIT_SUCCESS;
}