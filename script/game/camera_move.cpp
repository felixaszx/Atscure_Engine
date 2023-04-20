#include "engine/engine_wrapper.hpp"
#include "platform/platform_wrapper.hpp"

AS_SCRIPT void update(as::Entity* camera)
{
    auto camera_data = camera->get<as::CameraComp>();
    auto trans = camera->get<as::TransformComp>();
    auto hw = camera->get<as::GameScriptsComp>()->hardware_info_;

    float camera_speed = 15.0f;
    camera_data->pitch_ = std::clamp(camera_data->pitch_ - hw->mouse_.delta_y_ //
                                                               * camera_speed  //
                                                               * hw->delta_s_, //
                                     -79.0f, 79.0f);
    camera_data->yaw_ -= hw->mouse_.delta_x_ * camera_speed //
                         * hw->delta_s_;

    float speed = 15.0f;
    glm::vec3 front = {sin(glm::radians(camera_data->yaw_)) * cos(glm::radians(camera_data->pitch_)), //
                       sin(glm::radians(camera_data->pitch_)),                                        //
                       cos(glm::radians(camera_data->yaw_)) * cos(glm::radians(camera_data->pitch_))};
    front = glm::normalize(front);
    trans->trans_->front_ = front;

    glm::vec3 move_direction = glm::vec3(0.0f);
    if (glfwGetKey(hw->base_, GLFW_KEY_W) == GLFW_PRESS)
    {
        move_direction += front;
    }
    if (glfwGetKey(hw->base_, GLFW_KEY_S) == GLFW_PRESS)
    {
        move_direction -= front;
    }
    if (glfwGetKey(hw->base_, GLFW_KEY_D) == GLFW_PRESS)
    {
        move_direction += trans->trans_->right();
    }
    if (glfwGetKey(hw->base_, GLFW_KEY_A) == GLFW_PRESS)
    {
        move_direction -= trans->trans_->right();
    }
    if (glfwGetKey(hw->base_, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        trans->trans_->position_.y += speed * hw->delta_s_;
    }
    if (glfwGetKey(hw->base_, GLFW_KEY_GRAVE_ACCENT) == GLFW_PRESS)
    {
        trans->trans_->position_.y -= speed * hw->delta_s_;
    }
    
    if (glfwGetKey(hw->base_, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        speed *= 2;
    }
    if (glfwGetKey(hw->base_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(hw->base_, true);
    }

    if (move_direction != glm::vec3(0.0f))
    {
        trans->trans_->position_ += speed * hw->delta_s_ * glm::normalize(move_direction);
    }
}