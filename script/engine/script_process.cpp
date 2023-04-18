#include "engine/script_process.hpp"

void as::ScriptProcessor::call_func(Script::Func func_enum)
{
    for (std::vector<CallFunc>& funcs : script_maps_[func_enum])
    {
        for (CallFunc& func : funcs)
        {
            func.func(func.entity_);
        }
    }
}

void as::ScriptProcessor::check_scene(Scene& scene)
{
    for (int f = 0; f < as::Script::FUNC_COUNTS; f++)
    {
        for (std::vector<CallFunc>& funcs : script_maps_[f])
        {
            funcs.resize(0);
        }
    }

    auto view = scene.reg_.view<GameScriptsComp>();
    for (auto entity : view)
    {
        GameScriptsComp& script_comp = view.get<GameScriptsComp>(entity);
        for (int f = 0; f < Script::FUNC_COUNTS; f++)
        {
            for (int o = 0; o < AS_SCRIPT_MAX_ORDER; o++)
            {
                if (script_comp.scripts_[o] != nullptr)
                {
                    if (script_comp.scripts_[o]->funcs[f] != nullptr)
                    {
                        script_maps_[f][o].push_back({&script_comp.this_e_, script_comp.scripts_[o]->funcs[f]});
                    }
                }
            }
        }
    }
}

AS_SCRIPT void* read(void* data)
{
    return new as::ScriptProcessor;
}
