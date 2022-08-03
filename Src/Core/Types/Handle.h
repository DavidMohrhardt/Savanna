#pragma once

#include "Memory/CacheLine.h"

#include "Utilities/Macros/CppTypeDefs.h"
#include "Memory/CacheLine.h"

/**
 * @brief
 *
 */
typedef struct SAVANNA_CACHELINE_ALIGN
{
    union
    {
        // __se_intptr m_DataPtr;
        void* m_DataPtr;
        __se_byte m_HandleBytes[ sizeof(__se_intptr) ];
    };
} __se_Handle;
DECLARE_SAVANNA_NAMESPACED_CPP_TYPE_DEF(__se_Handle, Handle);


#ifdef __cplusplus

namespace Savanna
{
    template <typename T>
    class SEHandle
    {
    private:
        Handle m_Handle;

    public:
        SEHandle() : m_Handle({ nullptr }) {}

        SEHandle(T* ptr)
        {
            m_Handle.m_DataPtr = ptr;
        }

        SEHandle(const SEHandle& other)
            : m_Handle(other.m_Handle)
        {}

        SEHandle(SEHandle&& other)
        {
            if (this != other)
            {
                m_Handle = other.m_Handle;
                other.m_Handle.m_DataPtr = 0;
            }
        }

        SEHandle& operator=(const SEHandle& other) {
            if (this != &other)
            {
                m_Handle = other.m_Handle;
            }

            return *this;
        }

        SEHandle& operator=(SEHandle&& other) {
            if (this != &other)
            {
                m_Handle = other.m_Handle;
                other.m_Handle.m_DataPtr = 0;
            }
            return *this;
        }

        const Handle GetHandle() const { return m_Handle; }

        SAVANNA_NO_DISCARD T* Get() const SAVANNA_NO_EXCEPT
        {
            return reinterpret_cast<T*>( m_Handle.m_DataPtr );
        }

        const bool IsValid() const { return m_Handle.m_DataPtr != 0; }
    };
} // namespace Savanna

#endif // end __cplusplus