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
    as::Script engine_class(engine_dll);
    as::Script renderer_class(renderer_dll);
    as::Script mesh_class(mesh_dll);
    as::Script transfrom_class(transfrom_dll);

    as::Engine* engine = engine_class.create<as::Engine>();
    as::Renderer* renderer = renderer_class.create<as::Renderer>(engine);

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile("res/model/sponza/sponza.obj",
                                             aiProcess_Triangulate |    //
                                                 aiProcess_GenNormals | //
                                                 aiProcess_GenUVCoords);
    as::Mesh::CreateInfo mesh_cinfo{};
    mesh_cinfo.cmd_pool_ = renderer->cmd_pool_;
    mesh_cinfo.max_instance_ = 10;
    mesh_cinfo.scene_ = scene;
    as::Mesh* mesh = mesh_class.create<as::Mesh>(&mesh_cinfo);

    as::Transform* tt = transfrom_class.create<as::Transform>();
    as::Transform* tt2 = transfrom_class.create<as::Transform>();

    as::Scene render_scene;

    auto camera_e = render_scene.reg_.create();
    as::DynamicLoader camear_test("script/bin/game/game_script_camera_move.dll");
    as::Script camera_script(camear_test);
    camera_script.create<as::GameScriptComps>({camera_e, &render_scene.reg_});
    render_scene.reg_.emplace<as::CameraComp>(camera_e);
    render_scene.reg_.emplace<as::TransformComp>(camera_e).trans_ = tt;
    render_scene.reg_.emplace<as::GameScriptComps>(camera_e).scripts_.push_back(&camera_script);

    auto sponza = render_scene.reg_.create();
    as::DynamicLoader sponza_test("script/bin/game/game_script_sponza_scale.dll");
    as::Script sponza_script(sponza_test);
    sponza_script.create<as::GameScriptComps>({sponza, &render_scene.reg_});
    render_scene.reg_.emplace<as::MeshComp>(sponza).mesh_ = mesh;
    render_scene.reg_.emplace<as::TransformComp>(sponza).trans_ = tt2;
    render_scene.reg_.emplace<as::GameScriptComps>(sponza).scripts_.push_back(&sponza_script);

    while (!glfwWindowShouldClose(engine->window_->window_))
    {
        glfwPollEvents();

        auto view = render_scene.reg_.view<as::GameScriptComps>();
        for (auto entity : view)
        {
            auto script_comp = view.get<as::GameScriptComps>(entity).scripts_[0];
            script_comp->funcs[as::Script::UPDATE](nullptr);
        }

        renderer->render_scene(render_scene, engine->swapchian_->acquire_next_image(UINT64_MAX, *renderer->image_sem_));
        engine->swapchian_->present({*renderer->submit_sem_});

        renderer->wait_idle();
    }

    ffree(tt);
    ffree(mesh);
    ffree(renderer);
    ffree(engine);

    return EXIT_SUCCESS;
}