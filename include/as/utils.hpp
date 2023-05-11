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

        inline UniqueObj(T* obj_ptr)
            : std::unique_ptr<T>(obj_ptr)
        {
        }

        inline T& operator*() { return *this->get(); }
        inline operator T*() { return this->get(); }
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

        inline SharedObj(T* obj_ptr)
            : std::shared_ptr<T>(obj_ptr)
        {
        }

        inline T& operator*() { return *this->get(); }
        inline operator T*() { return this->get(); }
    };

    template <typename T>
    class VirtualObj
    {
      private:
        T* ptr_ = nullptr;

      public:
        inline VirtualObj() {}

        inline VirtualObj(T* obj_ptr)
            : ptr_(obj_ptr)
        {
        }

        inline VirtualObj(UniqueObj<T>& unique_obj)
            : ptr_(unique_obj)
        {
        }

        inline VirtualObj(SharedObj<T>& shared_obj)
            : ptr_(shared_obj)
        {
        }

        inline T& operator*() { return *ptr_; }
        inline operator T*() { return ptr_; }

        inline VirtualObj& operator=(const T*& obj_ptr)
        {
            ptr_ = obj_ptr;
            return *this;
        }

        inline VirtualObj& operator=(UniqueObj<T>& unique_obj)
        {
            ptr_ = unique_obj;
            return *this;
        }

        inline VirtualObj& operator=(SharedObj<T>& shared_obj)
        {
            ptr_ = shared_obj;
            return *this;
        }

        inline bool valide() { return this->ptr_ != nullptr; }
    };
}; // namespace as

#endif // UTILS_HPP
