#include "engine/engine_wrapper.hpp"
#include "platform/platform_wrapper.hpp"

AS_SCRIPT void start(as::Entity* sponza)
{
    sponza->get<as::TransformComp>()->trans_->scale_ = glm::vec3(0.1);
}