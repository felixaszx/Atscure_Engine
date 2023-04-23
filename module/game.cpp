#include "module/module_wrapper.hpp"
#include "as/as_wrapper.hpp"
#include "../script/camera_control.hpp"

int prev_scene_index = -1;
std::array<as::Scene, 1> scene{};
vk::Sampler sampler{};
as::Mesh* mm = nullptr;
const as::BaseModuleSingleton* base_in = nullptr;

as::Scene* load_scene(uint32_t scene_index)
{
    if (prev_scene_index >= 0)
    {
        scene[prev_scene_index].finish();
    }

    scene[scene_index].start();
    prev_scene_index = scene_index;

    return &scene[scene_index];
}

MODULE_EXPORT void destroy_module_single(GameModuleSingleton* obj)
{
    base_in->device_->destroySampler(sampler);
    ffree(mm);
}
MODULE_EXPORT void create_module_single(GameModuleSingleton* obj, const GameModuleSingleton::CreateInfo* base)
{
    base_in = base->base_;
    devicei = base->devicei_;
    obj->load_scene = load_scene;

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

    as::Entity sponza = scene[0].add_entity("sponza");
    sponza.add<as::MeshComp>().mesh_ = mm;
    auto& sponza_trans = sponza.add<as::TransformComp>().trans_;
    sponza_trans.push_back({});
    sponza_trans[0].scale_ = {0.1, 0.1, 0.1};

    as::Entity camera = scene[0].add_entity("camera");
    camera.add<as::CameraComp>();
    camera.add<as::TransformComp>().trans_.push_back({});
    camera.add<as::ScriptComp>().set<CameraControl>(camera);
}
