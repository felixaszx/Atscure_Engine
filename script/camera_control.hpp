#include "as/as_wrapper.hpp"
#include "module/module_wrapper.hpp"

class CameraControl : public as::ScriptBase
{
  private:
    int aaa;

  public:
    void update(float delta_t)
    {
        as::TransformComp* trans = e_.get<as::TransformComp>();
        if (devicei->keys_[GLFW_KEY_SPACE] >= as::Action::PRESS)
        {
            trans->trans_[0].position_ += as::Y_AXIS;
        }
    }
};