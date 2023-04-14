#ifndef LOADER_HPP
#define LOADER_HPP

#include <windows.h>
#include "api/logging.hpp"

namespace as
{
    struct WinDLL
    {
        HINSTANCE dll_libs;

        WinDLL(const std::string& dll_name);
        ~WinDLL();

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

}; // namespace as

#endif // LOADER_HPP
