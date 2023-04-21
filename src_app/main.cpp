#include <iostream>
#include <string>
#include "api/api_wrapper.hpp"
#include "platform/platform_wrapper.hpp"
#include "module/module_wrapper.hpp"
#include "as/as_wrapper.hpp"

int main(int argc, char** argv)
{
    as::DynamicLoader base_dll("module/bin/m_base.dll");
    as::ModuleSingleton<as::BaseModuleSingleton> base(base_dll, true);

    as::DynamicLoader renderer_dll("module/bin/m_renderer.dll");
    as::ModuleSingleton<as::RendererModuleSingleton> renderer(renderer_dll, base);

    vk::Sampler sampler;
    vk::SamplerCreateInfo sampler_cinfo{};
    sampler_cinfo.anisotropyEnable = true;
    sampler_cinfo.maxAnisotropy = 4;
    sampler_cinfo.borderColor = vk::BorderColor::eFloatOpaqueBlack;
    sampler_cinfo.maxLod = 1000.0f;
    sampler_cinfo.mipmapMode = vk::SamplerMipmapMode::eLinear;
    sampler_cinfo.magFilter = vk::Filter::eLinear;
    sampler_cinfo.minFilter = vk::Filter::eLinear;
    sampler = base.device_->createSampler(sampler_cinfo);

    Assimp::Importer importer;
    as::Mesh::CreateInfo mesh_cinfo;
    mesh_cinfo.cmd_pool_ = base.master_cmd_pool_;
    mesh_cinfo.path_ = "res/model/sponza";
    mesh_cinfo.sampler_ = sampler;
    mesh_cinfo.scene_ = importer.ReadFile("res/model/sponza/sponza.obj", //
                                          aiProcess_Triangulate | aiProcess_GenNormals);
    as::Mesh* mm = new as::Mesh(mesh_cinfo);

    ffree(mm);

    return EXIT_SUCCESS;
}