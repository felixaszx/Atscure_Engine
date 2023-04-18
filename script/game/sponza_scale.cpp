#include "engine/engine_wrapper.hpp"
#include "platform/platform_wrapper.hpp"

AS_SCRIPT void update(as::Entity* sponza)
{
    sponza->get<as::TransformComp>()->trans_->scale_ = glm::vec3(0.01 * (sin(glfwGetTime()) + 1));
}