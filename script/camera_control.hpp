#include "as/as_wrapper.hpp"
#include "module/module_wrapper.hpp"

class CameraControl : public as::ScriptBase
{
  public:
    void start() { devicei->set_cursor_state(false); }

    void update(float delta_t)
    {
        using namespace as;

        TransformComp* trans = e_.get<TransformComp>();
        CameraComp* camera = e_.get<CameraComp>();

        {
            float speed = 20.0f * delta_t;
            camera->pitch_ = std::clamp(camera->pitch_ - casts(float, speed * devicei->delta_mouse_.y_), //
                                        -79.0f, 79.0f);
            camera->yaw_ -= speed * devicei->delta_mouse_.x_;
        }
        {
            float speed = 15.0f * delta_t;
            trans->trans_[0].front_ = camera->get_front();
            glm::vec3 move(0.0f);

            if (devicei->keys_[GLFW_KEY_LEFT_SHIFT] >= Action::PRESS)
            {
                speed *= 2;
            }

            if (devicei->keys_[GLFW_KEY_W] >= Action::PRESS)
            {
                move += trans->trans_[0].front_;
            }
            if (devicei->keys_[GLFW_KEY_S] >= Action::PRESS)
            {
                move += -trans->trans_[0].front_;
            }
            if (devicei->keys_[GLFW_KEY_D] >= Action::PRESS)
            {
                move += trans->trans_[0].right();
            }
            if (devicei->keys_[GLFW_KEY_A] >= Action::PRESS)
            {
                move += -trans->trans_[0].right();
            }

            if (devicei->keys_[GLFW_KEY_SPACE] >= Action::PRESS)
            {
                trans->trans_[0].position_ += speed * Y_AXIS;
            }
            if (devicei->keys_[GLFW_KEY_GRAVE_ACCENT] >= Action::PRESS)
            {
                trans->trans_[0].position_ -= speed * Y_AXIS;
            }

            if (devicei->keys_[GLFW_KEY_ESCAPE].order_check(Action::PRESS, Action::RELEASE))
            {
                devicei->close_window();
            }

            if (move != glm::vec3(0.0f))
            {
                trans->trans_[0].position_ += speed * glm::normalize(move);
            }
        }
    }
};