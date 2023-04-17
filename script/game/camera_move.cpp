#include "engine/engine_wrapper.hpp"
#include "platform/platform_wrapper.hpp"

AS_GAME_SCRIPT(camera)

AS_SCRIPT void update(void* data)
{
    camera.get<as::CameraComp>()->pitch_ = -79.0f;
    camera.get<as::TransformComp>()->trans_->position_ = {0, 10, 0};
}