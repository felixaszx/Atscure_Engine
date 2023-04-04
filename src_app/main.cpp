#include "api/context.hpp"
#include "api/device.hpp"
#include "api/swapchain.hpp"

int main(int argc, char** argv)
{
    std::unique_ptr<as::Window> window(new as::Window(1920, 1080));
    auto context = std::make_unique<as::Context>(true);
    window->create_surface(*context);

    auto device = std::make_unique<as::Device>(*context, context->VALIDATION_LAYERS);
    as::Swapchain& swapchain = *device->link(new as::Swapchain(*window, *context, *device));

    delete device.release();
    delete context.release();
    delete window.release();

    return EXIT_SUCCESS;
}