#include "platform/script.hpp"

as::Script::Script(as::DynamicLoader& loader)
{
    funcs[INIT] = LoadFunc(loader, FUNC_NAMES[INIT]);
    funcs[FINISH] = LoadFunc(loader, FUNC_NAMES[FINISH]);
    funcs[WRITE] = LoadFunc(loader, FUNC_NAMES[WRITE]);
    funcs[READ] = LoadFunc(loader, FUNC_NAMES[READ]);

    funcs[START] = LoadFunc(loader, FUNC_NAMES[START]);
    funcs[END] = LoadFunc(loader, FUNC_NAMES[END]);
    funcs[UPDATE] = LoadFunc(loader, FUNC_NAMES[UPDATE]);
    funcs[FIXED] = LoadFunc(loader, FUNC_NAMES[FIXED]);
}