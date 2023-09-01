#ifndef MODULE_HPP
#define MODULE_HPP

#include "platform/loader.hpp"

#define MODULE_EXPORT extern "C" __declspec(dllexport)

namespace as
{
    template <typename T>
    class ModuleSingleton : public T
    {
      private:
        void (*create_module_single)(T*, const typename T::CreateInfo*);
        void (*destroy_module_single)(T*);

      public:
        ModuleSingleton(DynamicLoader& loader, const typename T::CreateInfo& create_info)
        {
            create_module_single = LoadFunc(loader, "create_module_single");
            destroy_module_single = LoadFunc(loader, "destroy_module_single");
            create_module_single(this, &create_info);
        }
        ~ModuleSingleton() { destroy_module_single(this); }
    };

}; // namespace as

#endif // MODULE_HPP
