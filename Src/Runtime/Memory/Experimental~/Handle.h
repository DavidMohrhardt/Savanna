#pragma once

#include <SavannaEngine.h>

#define SAVANNA_HANDLE_IS_POINTER 1

typedef se_uint64_t se_Handle_t;

#if __cplusplus

namespace Savanna
{
    static_assert(sizeof(se_Handle_t) != sizeof(void*), "Handle size must be at least the size of a pointer!");
    template <typename T>
    struct Handle
    {
        static Handle<T> NullHandle() { return Handle<T>(0x0); }

        template <typename ...Args>
        static Handle<T> AllocateHandle(Args&&... args);


        Handle() : m_Handle(0x0) {}
        Handle(const se_Handle_t& handle) : m_Handle(handle) {}
        Handle(const Handle<T>& handle) : m_Handle(handle.m_Handle) {}

        se_Handle_t& operator=(const se_Handle_t& handle)
        {
            m_Handle = handle;
            return *this;
        }

        se_Handle_t& operator=(const Handle<T>& handle)
        {
            m_Handle = handle.m_Handle;
            return *this;
        }

        se_Handle_t& operator=(const T* handle)
        {
            m_Handle = reinterpret_cast<se_Handle_t>(handle);
            return *this;
        }

        operator se_Handle_t() const { return m_Handle; }

        operator T*() const;

    private:
        se_Handle_t m_Handle;
    };

    template <typename T>
    template <typename... Args>
    inline Handle<T> Handle<T>::AllocateHandle(Args &&...args)
    {
#if SAVANNA_HANDLE_IS_POINTER
        return Handle<T>(reinterpret_cast<se_Handle_t>(MemoryManager::New<T>(std::forward<Args>(args)...)));
#else
        return MemoryManager::InitAndRegisterHandle<T>(m_Handle, std::forward<Args>(args)...);
#endif
    }

    template <typename T> inline Handle<T>::operator T *() const {
#if SAVANNA_HANDLE_IS_POINTER
        return reinterpret_cast<T*>(m_Handle);
#else
        return MemoryManager::ResolveHandle<T>(m_Handle);
#endif
    }

} // namespace Savanna

#endif // __cplusplus
