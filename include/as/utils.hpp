#ifndef UTILS_HPP
#define UTILS_HPP

#include <memory>

template <typename T>
class UniqueObj : public std::unique_ptr<T>
{
  public:
    template <typename... Args>
    constexpr inline UniqueObj(Args&&... args)
        : std::unique_ptr<T>(new T(std::forward<Args>(args)...))
    {
    }

    constexpr inline UniqueObj(T* obj_ptr)
        : std::unique_ptr<T>(obj_ptr)
    {
    }

    constexpr inline operator T&() { return *this->get(); }
    constexpr inline operator T*() { return this->get(); }
};

template <typename T>
class SharedObj : public std::shared_ptr<T>
{
  public:
    template <typename... Args>
    constexpr inline SharedObj(Args&&... args)
        : std::shared_ptr<T>(new T(std::forward<Args>(args)...))
    {
    }

    constexpr inline SharedObj(SharedObj<T>& shared_obj)
        : std::shared_ptr<T>(shared_obj)
    {
    }

    constexpr inline SharedObj(T* obj_ptr)
        : std::shared_ptr<T>(obj_ptr)
    {
    }

    constexpr inline operator T&() { return *this->get(); }
    constexpr inline operator T*() { return this->get(); }
};

template <typename T>
class VirtualObj
{
  private:
    T* ptr_ = nullptr;

  public:
    constexpr inline VirtualObj() {}

    constexpr inline VirtualObj(T* obj_ptr)
        : ptr_(obj_ptr)
    {
    }

    constexpr inline VirtualObj(UniqueObj<T>& unique_obj)
        : ptr_(unique_obj)
    {
    }

    constexpr inline VirtualObj(SharedObj<T>& shared_obj)
        : ptr_(shared_obj)
    {
    }

    constexpr inline operator T&() { return *this->get(); }
    constexpr inline operator T*() { return this->get(); }
    constexpr inline T& operator*() { return *ptr_; }
    constexpr inline T* operator->() { return ptr_; }

    constexpr inline VirtualObj& operator=(const T*& obj_ptr)
    {
        ptr_ = obj_ptr;
        return *this;
    }

    constexpr inline VirtualObj& operator=(UniqueObj<T>& unique_obj)
    {
        ptr_ = unique_obj;
        return *this;
    }

    constexpr inline VirtualObj& operator=(SharedObj<T>& shared_obj)
    {
        ptr_ = shared_obj;
        return *this;
    }
};

#endif // UTILS_HPP
