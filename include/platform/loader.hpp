#ifndef LOADER_HPP
#define LOADER_HPP

#include <windows.h>
#include "api/logging.hpp"

namespace as
{

#if defined(_WIN32)
    struct DynamicLoader
    {
        HINSTANCE dll_libs_;

        DynamicLoader(const std::string& dll_name);
        ~DynamicLoader();

        template <typename F>
        F get_function(const std::string& func_name)
        {
            if (dll_libs_ != nullptr)
            {
                return (F)GetProcAddress(dll_libs_, func_name.c_str());
            }

            return (F) nullptr;
        }
    };
#elif defined(__linux__)
    struct DynamicLoader
    {
    };
#else
    struct DynamicLoader
    {
    };
#endif

    class LoadFunc
    {
      private:
        DynamicLoader* loader_{};
        std::string name_{};

      public:
        LoadFunc(DynamicLoader& loader, const std::string& func_name);

        template <typename T>
        operator T()
        {
            return loader_->get_function<T>(name_);
        }
    };

}; // namespace as

#endif // LOADER_HPP
