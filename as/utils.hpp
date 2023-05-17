#ifndef AS_TOOLS_HPP
#define AS_TOOLS_HPP

#include <memory>
#include <cassert>

namespace as
{
    template <typename T>
    class BridgeObj;

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

        inline UniqueObj(T* ptr)
            : std::unique_ptr<T>(ptr)
        {
        }

        inline UniqueObj(BridgeObj<T>& bridge_obj)
            : std::unique_ptr<T>(bridge_obj.release())
        {
        }

        template <typename... Args>
        constexpr inline void operator()(Args&&... args)
        {
            *this = std::move(UniqueObj(std::forward<Args>(args)...));
        }

        inline operator T&() { return *this->get(); };

        inline void destroy()
        {
            if (this->get())
            {
                delete this->release();
            }
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

        inline SharedObj(T* ptr)
            : std::shared_ptr<T>(ptr)
        {
        }

        template <typename... Args>
        constexpr inline void operator()(Args&&... args)
        {
            *this = std::move(SharedObj(std::forward<Args>(args)...));
        }

        inline operator T&() { return *this->get(); };
    };

    template <typename T>
    class VirtualObj
    {
      private:
        T* ptr_ = nullptr;

      public:
        inline VirtualObj(T* obj_ptr = nullptr) { ptr_ = obj_ptr; }
        inline VirtualObj(T& obj) { ptr_ = &obj; };

        template <typename P>
        constexpr inline VirtualObj(const VirtualObj<P>& virtual_obj)
        {
            this->ptr_ = virtual_obj.ptr();
        }

        template <typename P>
        constexpr inline VirtualObj(UniqueObj<P>& virtual_obj)
        {
            this->ptr_ = virtual_obj.get();
        }

        template <typename P>
        constexpr inline VirtualObj(SharedObj<P>& virtual_obj)
        {
            this->ptr_ = virtual_obj.get();
        }

        inline T* operator->() { return ptr_; }
        inline T& operator*() { return *ptr_; }
        inline operator T&() { return *ptr_; };

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
        inline T* ptr() const { return ptr_; }
    };

    template <typename T>
    struct FreeObj : public std::unique_ptr<T>
    {
        template <typename... Args>
        constexpr inline FreeObj(Args&&... args)
            : std::unique_ptr<T>(new T(std::forward<Args>(args)...))
        {
        }

        inline FreeObj(std::nullptr_t null)
            : std::unique_ptr<T>(null)
        {
        }

        inline FreeObj(BridgeObj<T>& bridge_obj)
            : std::unique_ptr<T>(bridge_obj.release())
        {
        }

        inline FreeObj(FreeObj& free_obj)
        {
            T* data = this->release();
            if (data)
            {
                delete data;
            }
            this->reset(free_obj.release());
        }

        template <typename... Args>
        constexpr inline void operator()(Args&&... args)
        {
            *this = std::move(FreeObj(std::forward<Args>(args)...));
        }

        inline operator T&() { return *this->get(); };

        inline FreeObj& operator=(FreeObj& free_obj)
        {
            T* data = this->release();
            if (data)
            {
                delete data;
            }
            this->reset(free_obj.release());

            return *this;
        }

        inline void destroy()
        {
            if (this->get())
            {
                delete this->release();
            }
        }
    };

    template <typename T>
    class BridgeObj
    {
      private:
        T* ptr_ = nullptr;

      public:
        inline BridgeObj(T* ptr = nullptr) { ptr_ = ptr; }

        inline BridgeObj(UniqueObj<T>& unique_obj)
        {
            ptr_ = unique_obj.release();
            unique_obj.reset();
        }

        inline BridgeObj(FreeObj<T>& free_obj)
        {
            ptr_ = free_obj.release();
            free_obj.reset();
        }

        inline ~BridgeObj()
        {
            assert(!ptr_ && "This bridge still hold the ownership of pointer");
            if (ptr_)
            {
                delete ptr_;
            }
        }

        inline operator T*() { return release(); }
        inline bool empty() { return ptr_ == nullptr; }

        inline T* release()
        {
            T* tmp = ptr_;
            ptr_ = nullptr;
            return tmp;
        };
    };

}; // namespace as

#endif // AS_TOOLS_HPP
