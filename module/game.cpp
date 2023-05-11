#include "module/module_wrapper.hpp"
#include "as/as_wrapper.hpp"
#include "../script/camera_control.hpp"

int prev_scene_index = -1;
std::array<as::Scene*, 1> scene{};
const as::BaseModuleSingleton* base_in = nullptr;

as::Scene* load_scene(uint32_t scene_index)
{
    if (prev_scene_index >= 0)
    {
        scene[prev_scene_index]->finish();
    }

    scene[scene_index]->start();
    prev_scene_index = scene_index;

    return scene[scene_index];
}

MODULE_EXPORT void destroy_scene(uint32_t scene_index)
{
    if (scene_index < scene.size())
    {
        scene[scene_index]->finish();
        ffree(scene[scene_index]);
    }
}

vk::Sampler sampler{};
MODULE_EXPORT void create_module_single(as::GameModuleSingleton* obj, const as::GameModuleSingleton::CreateInfo* base)
{
    base_in = base->base_;
    as::devicei = base->devicei_;
    obj->load_scene = load_scene;
    obj->destroy_scene = destroy_scene;

    for (int i = 0; i < scene.size(); i++)
    {
        scene[i] = new as::Scene;
    }

    vk::SamplerCreateInfo sampler_cinfo{};
    sampler_cinfo.anisotropyEnable = true;
    sampler_cinfo.maxAnisotropy = 4;
    sampler_cinfo.borderColor = vk::BorderColor::eFloatOpaqueBlack;
    sampler_cinfo.maxLod = 1000.0f;
    sampler_cinfo.mipmapMode = vk::SamplerMipmapMode::eLinear;
    sampler_cinfo.magFilter = vk::Filter::eLinear;
    sampler_cinfo.minFilter = vk::Filter::eLinear;
    sampler = base_in->device_->createSampler(sampler_cinfo);

    as::Entity physic_world = scene[0]->add_entity();
    physic_world.add<as::PhysicalWorldComp>();

    Assimp::Importer importer;
    as::Mesh::CreateInfo mesh_cinfo;
    mesh_cinfo.cmd_pool_ = base_in->master_cmd_pool_;
    mesh_cinfo.sampler_ = sampler;
    mesh_cinfo.path_ = "res/model/sponza";
    mesh_cinfo.scene_ = importer.ReadFile("res/model/sponza/sponza.obj", //
                                          aiProcess_Triangulate | aiProcess_GenNormals);

    as::Entity sponza = scene[0]->add_entity();
    sponza.add<as::MeshComp>().mesh_ = std::make_unique<as::Mesh>(mesh_cinfo);
    auto& sponza_trans = sponza.add<as::TransformComp>().trans_;
    sponza_trans.push_back({});
    sponza.add<as::ScriptComp>().set<SponzaSize>(sponza);

    mesh_cinfo.path_ = "res/model/cube";
    mesh_cinfo.scene_ = importer.ReadFile("res/model/cube/cube.obj", //
                                          aiProcess_Triangulate | aiProcess_GenNormals);
    as::Entity cube = scene[0]->add_entity();
    cube.add<as::MeshComp>().mesh_ = std::make_unique<as::Mesh>(mesh_cinfo);
    auto& cube_trans = cube.add<as::TransformComp>().trans_;
    cube_trans.push_back({});
    cube.add<as::ScriptComp>().set<CubeTest>(cube);

    as::Entity camera = scene[0]->add_entity();
    camera.add<as::TransformComp>().trans_.push_back({});
    camera.add<as::ScriptComp>().set<CameraControl>(camera);
    camera.add<as::CameraComp>();
}

MODULE_EXPORT void destroy_module_single(as::GameModuleSingleton* obj)
{
    base_in->device_->destroySampler(sampler);
    for (int i = 0; i < scene.size(); i++)
    {
        if (scene[i] != nullptr)
        {
            ffree(scene[i]);
        }
    }
}
