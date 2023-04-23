#ifndef SCENE_HPP
#define SCENE_HPP

#include <btBulletDynamicsCommon.h>
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
        std::string id_ = "";

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
        btDiscreteDynamicsWorld* world = nullptr;

        Entity add_entity(const std::string& id = "");
        void start();
        void finish();
        void update(float delta_t);
        void fix_update();

        void check_physis();
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
        Mesh* mesh_ = nullptr;
    };

    struct RigidBodyComp
    {
        // in kg
        float mass_ = 1;
        btRigidBody* body_ = nullptr;
    };

}; // namespace as

#endif // SCENE_HPP
