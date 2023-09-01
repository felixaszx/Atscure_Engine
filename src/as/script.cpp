#include "as/script.hpp"

void as::DeviceI::close_window() const
{
    glfwSetWindowShouldClose(window_, true);
}

void as::DeviceI::set_cursor_state(bool enable) const
{
    glfwSetInputMode(window_, GLFW_CURSOR, //
                     enable ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}
