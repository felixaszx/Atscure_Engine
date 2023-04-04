#include "api/context.hpp"

int main(int argc, char** argv)
{
    auto window = std::make_unique<as::Window>(1920, 1080);
    auto context = std::make_unique<as::Context>(true);
    window->create_surface(*context);

    delete context.release();
    delete window.release();

    return EXIT_SUCCESS;
}