#include "platform/loader.hpp"

as::WinDLL::WinDLL(const std::string& dll_name)
{
    dll_libs = LoadLibraryA(dll_name.c_str());
}

as::WinDLL::~WinDLL()
{
    if (dll_libs != nullptr)
    {
        FreeLibrary(dll_libs);
    }
}
