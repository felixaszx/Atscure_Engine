#include "platform/script.hpp"

as::Script::Script(as::DynamicLoader& loader)
{
    for (int i = 0; i < FUNC_COUNTS; i++)
    {
        funcs[i] = LoadFunc(loader, FUNC_NAMES[i]);
    }
}