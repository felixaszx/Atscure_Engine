#include <iostream>
#include "render/render.hpp"

int main(int argc, char** argv)
{
    as::RenderContext rc(1920, 1080);

    as::DefferedProgram dp(1920, 1080);

    while (rc.running())
    {
        rc.poll_events();

        dp.run();
    }
    return EXIT_SUCCESS;
}