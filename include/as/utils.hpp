#ifndef UTILS_HPP
#define UTILS_HPP

#include <memory>

namespace as
{
    template <typename T>
    struct UniqueObj : public std::unique_ptr<T>
    {
        template <typename... Args>
        constexpr inline UniqueObj(Args&&... args)
            : std::unique_ptr<T>(new T(std::forward<Args>(args)...))
        {
        }

        inline UniqueObj(std::nullptr_t null)
            : std::unique_ptr<T>(null)
        {
        }

        template <typename... Args>
        constexpr inline void operator()(Args&&... args)
        {
            *this = std::move(UniqueObj(std::forward<Args>(args)...));
        }
    };

    template <typename T>
    struct SharedObj : public std::shared_ptr<T>
    {
        template <typename... Args>
        constexpr inline SharedObj(Args&&... args)
            : std::shared_ptr<T>(new T(std::forward<Args>(args)...))
        {
        }

        inline SharedObj(SharedObj<T>& shared_obj)
            : std::shared_ptr<T>(shared_obj)
        {
        }

        inline SharedObj(std::nullptr_t null)
            : std::shared_ptr<T>(null)
        {
        }
    };

    template <typename T>
    class VirtualObj
    {
      private:
        T* ptr_ = nullptr;

      public:
        inline VirtualObj(T* obj_ptr = nullptr) { ptr_ = obj_ptr; }
        inline VirtualObj(T& obj) { ptr_ = &obj; };
        inline VirtualObj(UniqueObj<T>& unique_obj) { ptr_ = unique_obj.get(); }
        inline VirtualObj(SharedObj<T>& shared_obj) { ptr_ = shared_obj.get(); }

        inline T* operator->() { return ptr_; }
        inline T& operator*() { return *ptr_; }

        inline VirtualObj& operator=(const T*& obj_ptr)
        {
            ptr_ = obj_ptr;
            return *this;
        }

        inline VirtualObj& operator=(UniqueObj<T>& unique_obj)
        {
            ptr_ = unique_obj.get();
            return *this;
        }

        inline VirtualObj& operator=(SharedObj<T>& shared_obj)
        {
            ptr_ = shared_obj.get();
            return *this;
        }

        inline bool valide() { return ptr_ != nullptr; }
    };

#define PIMPL_STRUCT(type_name, field_name) \
    struct type_name;                       \
    std::unique_ptr<type_name> field_name;
}; // namespace as

#endif // UTILS_HPP
