#include <iostream>
#include <string>
#include "api/api_wrapper.hpp"
#include "platform/platform_wrapper.hpp"
#include "engine/engine_wrapper.hpp"

int main(int argc, char** argv)
{
    as::DynamicLoader engine_dll("script/bin/engine/engine_script_engine.dll");
    as::DynamicLoader renderer_dll("script/bin/engine/engine_script_renderer.dll");
    as::DynamicLoader mesh_dll("script/bin/engine/engine_script_mesh.dll");
    as::DynamicLoader transfrom_dll("script/bin/engine/engine_script_transform.dll");
    as::DynamicLoader script_process_dll("script/bin/engine/engine_script_script_process.dll");
    as::DynamicLoader texture_dll("script/bin/engine/engine_script_texture.dll");
    as::Script engine_class(engine_dll);
    as::Script renderer_class(renderer_dll);
    as::Script mesh_class(mesh_dll);
    as::Script transfrom_class(transfrom_dll);
    as::Script script_processor_class(script_process_dll);
    as::Script texture_class(texture_dll);

    as::Engine* engine = engine_class.load<as::Engine>();
    as::Renderer* renderer = renderer_class.create<as::Renderer>(engine);
    as::ScriptProcessor* sprocessor = script_processor_class.load<as::ScriptProcessor>();

    vk::Sampler sampler;
    vk::SamplerCreateInfo sampler_info{};
    sampler_info.mipmapMode = vk::SamplerMipmapMode::eLinear;
    sampler_info.anisotropyEnable = true;
    sampler_info.maxAnisotropy = 4.0f;
    sampler_info.borderColor = vk::BorderColor::eFloatOpaqueBlack;
    sampler_info.minFilter = vk::Filter::eLinear;
    sampler_info.magFilter = vk::Filter::eLinear;
    sampler_info.maxLod = 1000.0f;
    sampler = engine->device_->createSampler(sampler_info);

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile("res/model/sponza/sponza.obj",
                                             aiProcess_Triangulate |    //
                                                 aiProcess_GenNormals | //
                                                 aiProcess_GenUVCoords);

    as::Mesh::CreateInfo mesh_cinfo{};
    mesh_cinfo.cmd_pool_ = renderer->cmd_pool_;
    mesh_cinfo.scene_ = scene;
    mesh_cinfo.texture_class_ = &texture_class;
    mesh_cinfo.path_ = "res/model/sponza";
    mesh_cinfo.sampler_ = sampler;
    as::Mesh* mesh = mesh_class.create<as::Mesh>(&mesh_cinfo);

    as::Transform* tt = transfrom_class.load<as::Transform>();
    as::Transform* tt2 = transfrom_class.load<as::Transform>();

    as::Scene render_scene;

    as::CpuTimer timer;
    as::HardwareInfo hw_info{};
    hw_info.base_ = engine->window_->window_;
    glfwGetCursorPos(engine->window_->window_, &hw_info.mouse_.curr_x_, &hw_info.mouse_.curr_y_);
    glfwGetWindowSize(engine->window_->window_, &hw_info.window_.w_, &hw_info.window_.h_);
    glfwSetInputMode(engine->window_->window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    auto camera_e = render_scene.reg_.create();
    as::DynamicLoader camear_test("script/bin/game/game_script_camera_move.dll");
    as::Script camera_script(camear_test);
    render_scene.reg_.emplace<as::CameraComp>(camera_e);
    render_scene.reg_.emplace<as::TransformComp>(camera_e) = {tt};
    render_scene.reg_.emplace<as::GameScriptsComp>(camera_e) = {{camera_e, &render_scene.reg_}, //
                                                                {&camera_script},
                                                                &hw_info};

    auto sponza = render_scene.reg_.create();
    as::DynamicLoader sponza_test("script/bin/game/game_script_sponza_scale.dll");
    as::Script sponza_script(sponza_test);
    render_scene.reg_.emplace<as::MeshComp>(sponza) = {mesh};
    render_scene.reg_.emplace<as::TransformComp>(sponza) = {tt2};
    render_scene.reg_.emplace<as::GameScriptsComp>(sponza) = {{sponza, &render_scene.reg_}, //
                                                              {nullptr, &sponza_script},
                                                              &hw_info};

    sprocessor->check_scene(render_scene);

    sprocessor->call_func(as::Script::START);
    while (!glfwWindowShouldClose(engine->window_->window_))
    {
        timer.start();
        glfwPollEvents();

        hw_info.mouse_.prev_x_ = hw_info.mouse_.curr_x_;
        hw_info.mouse_.prev_y_ = hw_info.mouse_.curr_y_;
        glfwGetCursorPos(engine->window_->window_, &hw_info.mouse_.curr_x_, &hw_info.mouse_.curr_y_);
        hw_info.mouse_.delta_x_ = hw_info.mouse_.curr_x_ - hw_info.mouse_.prev_x_;
        hw_info.mouse_.delta_y_ = hw_info.mouse_.curr_y_ - hw_info.mouse_.prev_y_;
        glfwGetWindowSize(engine->window_->window_, &hw_info.window_.w_, &hw_info.window_.h_);

        sprocessor->check_scene(render_scene);
        sprocessor->call_func(as::Script::UPDATE);

        renderer->render_scene(render_scene, engine->swapchian_->acquire_next_image(UINT64_MAX, *renderer->image_sem_));
        engine->swapchian_->present({*renderer->submit_sem_});

        renderer->wait_idle();

        timer.finish();
        hw_info.delta_s_ = timer.get_duration_second();
        hw_info.delta_ms_ = timer.get_duration_ms();
    }
    sprocessor->call_func(as::Script::END);

    engine->device_->destroySampler(sampler);
    ffree(tt);
    ffree(mesh);
    ffree(renderer);
    ffree(engine);

    return EXIT_SUCCESS;
}