#include "module/module_wrapper.hpp"

class CameraControl : public as::ScriptBase
{
  private:
    int aaa;

  public:
    void update(float delta_t)
    {
        as::TransformComp* trans = e_.get<as::TransformComp>();
        if (glfwGetKey(window_, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            trans->trans_[0].position_ += as::Y_AXIS;
        }
    }
};

as::Scene scene{};
vk::Sampler sampler{};
as::Mesh* mm = nullptr;
const as::BaseModuleSingleton* base_in = nullptr;

as::Scene* load_scene()
{
    vk::SamplerCreateInfo sampler_cinfo{};
    sampler_cinfo.anisotropyEnable = true;
    sampler_cinfo.maxAnisotropy = 4;
    sampler_cinfo.borderColor = vk::BorderColor::eFloatOpaqueBlack;
    sampler_cinfo.maxLod = 1000.0f;
    sampler_cinfo.mipmapMode = vk::SamplerMipmapMode::eLinear;
    sampler_cinfo.magFilter = vk::Filter::eLinear;
    sampler_cinfo.minFilter = vk::Filter::eLinear;
    sampler = base_in->device_->createSampler(sampler_cinfo);

    Assimp::Importer importer;
    as::Mesh::CreateInfo mesh_cinfo;
    mesh_cinfo.cmd_pool_ = base_in->master_cmd_pool_;
    mesh_cinfo.path_ = "res/model/sponza";
    mesh_cinfo.sampler_ = sampler;
    mesh_cinfo.scene_ = importer.ReadFile("res/model/sponza/sponza.obj", //
                                          aiProcess_Triangulate | aiProcess_GenNormals);
    mm = new as::Mesh(mesh_cinfo);

    as::Entity sponza = scene.add_entity();
    sponza.add<as::MeshComp>().mesh_ = mm;
    sponza.add<as::TransformComp>().trans_.push_back({});

    as::Entity camera = scene.add_entity();
    camera.add<as::CameraComp>();
    camera.add<as::TransformComp>().trans_.push_back({});
    camera.add<as::ScriptComp>().set<CameraControl>(camera);

    return &scene;
}

MODULE_EXPORT void destroy_module_single(GameModuleSingleton* obj)
{
    base_in->device_->destroySampler(sampler);
    ffree(mm);
}

MODULE_EXPORT void create_module_single(GameModuleSingleton* obj, const as::BaseModuleSingleton* base)
{
    base_in = base;
    obj->load_scene = load_scene;
}
