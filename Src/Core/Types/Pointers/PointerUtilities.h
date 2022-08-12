#pragma once

#include "UniversalPointer.h"

#if __cplusplus

template<typename T>
inline T* Add(T* ptr, s_intptr offset)
{
    return (T*)((s_byte*)ptr + offset);
}

template<typename T>
inline T* Sub(T* ptr, s_intptr offset)
{
    return (T*)((s_byte*)ptr - offset);
}

template<typename T>
inline T* Add(T* ptr, s_uintptr offset)
{
    return (T*)((s_byte*)ptr + offset);
}

template<typename T>
inline T* Sub(T* ptr, s_uintptr offset)
{
    return (T*)((s_byte*)ptr - offset);
}

#else

inline void* Add(const void* const, const __se_intptr& offset)
{
    return (s_universalptr){ .m_UIntPtr = ptr.m_UIntPtr + offset }.m_Ptr;
}

inline void* Sub(const void* const ptr, const __se_intptr& offset)
{
    return (s_universalptr){ .m_UIntPtr = ptr.m_UIntPtr - offset }.m_Ptr;
}

#endif // end __cplusplus
