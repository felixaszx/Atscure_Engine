#include "engine/engine_wrapper.hpp"
#include "platform/platform_wrapper.hpp"

AS_SCRIPT void update(as::Entity* camera)
{
    camera->get<as::CameraComp>()->pitch_ = -79.0f;
    camera->get<as::TransformComp>()->trans_->position_ = {0, 10, 0};
}