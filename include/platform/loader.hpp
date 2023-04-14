#ifndef LOADER_HPP
#define LOADER_HPP

#include <windows.h>
#include "api/logging.hpp"

namespace
{
    struct WinDLL
    {
        HINSTANCE dll_libs;

        WinDLL(const std::string& dll_name) { dll_libs = LoadLibraryA(dll_name.c_str()); }
        ~WinDLL()
        {
            if (dll_libs != nullptr)
            {
                FreeLibrary(dll_libs);
            }
        }

        template <typename F>
        F get_function(const std::string& func_name)
        {
            if (dll_libs != nullptr)
            {
                return (F)GetProcAddress(dll_libs, func_name.c_str());
            }

            return (F) nullptr;
        }
    };

    struct NixDLL
    {
    };

    struct UnsopportedPlatform
    {
    };
}; // namespace

namespace as
{

#if defined(_WIN32)
    using DynamicLoader = WinDLL;
#elif defined(__linux__)
    using DynamicLoader = NixDLL;
#else
    using DynamicLoader = UnsopportedPlatform;
#endif

}; // namespace as

#endif // LOADER_HPP
