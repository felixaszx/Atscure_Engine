#include "engine/engine_wrapper.hpp"
#include "platform/platform_wrapper.hpp"

AS_GAME_SCRIPT(sponza)

AS_SCRIPT void update(void* data)
{
    sponza.get<as::TransformComp>()->trans_->scale_ = glm::vec3(0.01 * (sin(glfwGetTime()) + 1));
}