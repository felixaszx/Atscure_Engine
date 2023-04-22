#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include "scene.hpp"
#include "key_bits.hpp"

namespace as
{
    template <typename T>
    struct XY
    {
        T x_{};
        T y_{};
    };

    enum class Action
    {
        RELEASE = GLFW_RELEASE,
        PRESS = GLFW_PRESS,
        HOLD = GLFW_REPEAT
    };

    enum class ModKey
    {
        SHIFT,
        CTRL,
        ALT,
    };

    struct KeyCode
    {
        Action prev_ = Action::RELEASE;
        Action curr_ = Action::RELEASE;

        bool order_check(Action first, Action second) const { return (prev_ == first) && (curr_ == second); }
        bool operator==(const Action& action) const { return curr_ == action; }
        bool operator>=(const Action& action) const { return curr_ >= action; }
        bool operator<=(const Action& action) const { return curr_ <= action; }
        operator bool() const { return curr_ >= Action::PRESS; }
    };

    struct DeviceI
    {
        GLFWwindow* window_ = nullptr;

        KeyCode keys_[350]{};

        using MousePos = XY<double>;
        using MouseCode = KeyCode;
        MousePos prev_mouse_{};
        MousePos curr_mouse_{};
        MousePos delta_mouse_{};
        MouseCode mouse_press_[8]{};

        using ScrollOffset = XY<double>;
        ScrollOffset scroll_{};

        using MiliSecond = uint32_t;
        MiliSecond frame_time_ = 0;

        void close_window() const;
        void set_cursor_state(bool enable) const;
    };

    class_has_(update);
    class_has_(fix_update);
    class_has_(start);
    class_has_(finish);

    struct ScriptBase
    {
        Entity e_;
    };

    struct ScriptComp
    {
        ScriptBase* class_;

        void (*update)(ScriptBase* base, float delta_t) = nullptr;
        void (*fix_update)(ScriptBase* base) = nullptr;
        void (*start)(ScriptBase* base) = nullptr;
        void (*finish)(ScriptBase* base) = nullptr;

        template <typename S>
        void set(const Entity& e)
        {
            class_ = new S;
            class_->e_ = e;

            if constexpr (class_has_update<S>)
            {
                update = [](ScriptBase* base, float delta_t) { ((S*)(base))->update(delta_t); };
            }

            if constexpr (class_has_fix_update<S>)
            {
                fix_update = [](ScriptBase* base) { ((S*)(base))->fix_update(); };
            }

            if constexpr (class_has_start<S>)
            {
                start = [](ScriptBase* base) { ((S*)(base))->start(); };
            }

            if constexpr (class_has_finish<S>)
            {
                finish = [](ScriptBase* base) { ((S*)(base))->finish(); };
            }
        }

        ~ScriptComp() { ffree(class_); }
    };

}; // namespace as

#endif // SCRIPT_HPP
