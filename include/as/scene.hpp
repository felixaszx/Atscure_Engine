#ifndef SCENE_HPP
#define SCENE_HPP

#include "../ode/ode.h"
#include <entt/entt.hpp>
#include "mesh.hpp"
#include "transform.hpp"

namespace as
{
    class Entity
    {
      private:
        entt::entity e_{};
        entt::registry* reg_{};

      public:
        Entity();
        Entity(entt::entity e, entt::registry* reg);

        template <typename Component>
        Component* get()
        {
            if (reg_->valid(e_))
            {
                return reg_->try_get<Component>(e_);
            }
            return nullptr;
        }

        template <typename Component>
        Component& add()
        {
            return reg_->emplace_or_replace<Component>(e_);
        }

        template <typename Component>
        void remove()
        {
            reg_->remove<Component>(e_);
        }

        void self_destroy() { reg_->destroy(e_); }
    };

#define SCRIPT_FUNC_CALLER(func_name, ...)          \
    auto view = reg_.view<ScriptComp>();            \
    for (auto e : view)                             \
    {                                               \
        ScriptComp& comp = view.get<ScriptComp>(e); \
        if (comp.func_name != nullptr)              \
        {                                           \
            comp.func_name(__VA_ARGS__);            \
        }                                           \
    }

    struct Scene
    {
        entt::registry reg_{};

        Entity add_entity();
        void start();
        void finish();
        void update(float delta_t);
        void fix_update();
    };

    struct CameraComp
    {
        float yaw_ = 0.0f;
        float pitch_ = 0.0f;
        float fov_ = 45.0f;

        float near_ = 0.5f;
        float far_ = 1000.0f;
        float aspect_ = 1920.0f / 1080.0f;

        glm::vec3 get_front();
    };

    struct TransformComp
    {
        std::vector<Transform> trans_{};
    };

    struct MeshComp
    {
        std::unique_ptr<Mesh> mesh_{};
    };

    using Kg = float;
    using Meter = float;
    Kg operator"" _kg(long double);
    Meter operator"" _m(long double);
    struct RigidBodyComp
    {
        inline static const int64_t ALL_INSTANCE = -1;
        std::vector<dBody> bodies_{};
        std::vector<dMass> masses_{};
        std::vector<std::unique_ptr<dGeom>> geoms_{};

        template <typename T>
        void set(const TransformComp& trans, int64_t index = ALL_INSTANCE)
        {
        }

        glm::vec3 get_position(uint32_t index);
        glm::quat get_rotation(uint32_t index);
        void set_position(uint32_t index, const glm::vec3& vec3);
        void set_rotation(uint32_t index, const glm::quat& quat);
        std::vector<glm::vec3>& get_position();
        std::vector<glm::quat>& get_rotation();
        void set_position(const std::vector<glm::vec3>& vec3);
        void set_rotation(const std::vector<glm::quat>& quat);

        void sync_to_transform(TransformComp& trans);
    };
}; // namespace as

#endif // SCENE_HPP
