#include "platform/loader.hpp"

as::DynamicLoader::DynamicLoader(const std::string& dll_name)
{
    dll_libs_ = LoadLibraryA(dll_name.c_str());
}

as::DynamicLoader::~DynamicLoader()
{
    if (dll_libs_ != nullptr)
    {
        FreeLibrary(dll_libs_);
    }
}

as::LoadFunc::LoadFunc(DynamicLoader& loader, const std::string& func_name)
{
    loader_ = &loader;
    name_ = func_name;
}
