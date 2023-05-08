#include "as/as_wrapper.hpp"
#include "module/module_wrapper.hpp"

class CameraControl : public as::ScriptBase
{
  public:
    void start() { as::devicei->set_cursor_state(false); }

    void update(float delta_t)
    {
        using namespace as;

        TransformComp* trans = e_.get<TransformComp>();
        CameraComp* camera = e_.get<CameraComp>();

        if (devicei->keys_[KEY::LEFT_ALT])
        {
            devicei->set_cursor_state(true);
        }
        else if (!devicei->keys_[KEY::LEFT_ALT])
        {
            devicei->set_cursor_state(false);
            float speed = 20.0f * delta_t;
            camera->pitch_ = std::clamp(camera->pitch_ - casts(float, speed * devicei->delta_mouse_.y_), //
                                        -79.0f, 79.0f);
            camera->yaw_ -= speed * devicei->delta_mouse_.x_;
        }

        {
            float speed = 15.0f * delta_t;
            trans->trans_[0].front_ = camera->get_front();
            glm::vec3 move(0.0f);

            if (devicei->keys_[KEY::LEFT_SHIFT])
            {
                speed *= 2;
            }

            if (devicei->keys_[KEY::W])
            {
                move += trans->trans_[0].front_;
            }
            if (devicei->keys_[KEY::S])
            {
                move += -trans->trans_[0].front_;
            }
            if (devicei->keys_[KEY::D])
            {
                move += trans->trans_[0].right();
            }
            if (devicei->keys_[KEY::A])
            {
                move += -trans->trans_[0].right();
            }

            if (devicei->keys_[KEY::SPACE])
            {
                trans->trans_[0].position_ += speed * Y_AXIS;
            }
            if (devicei->keys_[KEY::GRAVE_ACCENT])
            {
                trans->trans_[0].position_ -= speed * Y_AXIS;
            }

            if (devicei->keys_[KEY::TAB])
            {
                camera->render_id_ = "a";
            }
            else
            {
                camera->render_id_ = "";
            }

            if (devicei->keys_[KEY::ESCAPE].short_release())
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

class SponzaSize : public as::ScriptBase
{
  public:
    void start() { this->e_.get<as::TransformComp>()->trans_[0].scale_ = {0.1f, 0.1f, 0.1f}; }
};