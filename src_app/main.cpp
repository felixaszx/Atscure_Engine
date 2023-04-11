#include <iostream>
#include <string>
#include "api/context.hpp"
#include "api/device.hpp"
#include "api/swapchain.hpp"
#include "api/cmd.hpp"
#include "api/syncs.hpp"
#include "api/buffer.hpp"
#include "api/descriptor.hpp"
#include "api/pipeline.hpp"
#include "api/render_pass.hpp"

int main(int argc, char** argv)
{
    as::Window window(1920, 1080);
    as::Context context(true);
    window.create_surface(context);

    as::Device device(context, context.VALIDATION_LAYERS);
    as::Swapchain swapchain(window, context, device);




    return EXIT_SUCCESS;
}