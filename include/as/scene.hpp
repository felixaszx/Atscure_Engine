#ifndef SCENE_HPP
#define SCENE_HPP

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

        template <typename Component, typename... Args>
        Component& add(const Args&... args)
        {
            return reg_->emplace_or_replace<Component>(e_, args...);
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

    struct PhysicalWorldComp;
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
        std::string render_id_ = "";

        float yaw_ = 0.0f;
        float pitch_ = 0.0f;
        float fov_ = 45.0f;

        float near_ = 0.5f;
        float far_ = 1000.0f;

        uint32_t width_ = 1920;
        uint32_t height_ = 1080;

        float x_ = 0.0; // normalized, 0.0f to 1.0f
        float y_ = 0.0; // normalized, 0.0f to 1.0f

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

}; // namespace as

#endif // SCENE_HPP
