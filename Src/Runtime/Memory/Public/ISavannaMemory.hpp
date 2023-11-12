/**
 * @file ISavannaMemory.hpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
 * @version 0.1
 * @date 2023-11-08
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef I_SAVANNA_MEMORY_HPP
#define I_SAVANNA_MEMORY_HPP

#include "ISavannaMemory.h"
#include "Public/ISavannaEngine.hpp"

#include <new>

#undef SAVANNA_NEW
#define SAVANNA_NEW(__allocatorKind, __type, ...) \
    new (SAVANNA_MALLOC_ALIGNED(__allocatorKind, sizeof(__type), alignof(__type))) __type(__VA_ARGS__)

#undef SAVANNA_NEW_ARRAY
#define SAVANNA_NEW_ARRAY(__allocatorKind, __type, __count) \
    new (SAVANNA_MALLOC_ALIGNED(__allocatorKind, sizeof(__type) * __count, alignof(__type))) __type[__count]

#undef SAVANNA_DELETE
#define SAVANNA_DELETE(__allocatorKind, __ptr) \
    Savanna::Delete(__ptr, __allocatorKind);

#undef SAVANNA_DELETE_ARRAY
#define SAVANNA_DELETE_ARRAY(__allocatorKind, __ptr) \
    Savanna::Delete(__ptr, __allocatorKind);

namespace Savanna
{
    using AllocatorKind = se_AllocatorKind_t;

    template <class T>
    inline void Delete(T* ptr, AllocatorKind allocatorKind)
    {
        ptr->~T();
        SAVANNA_FREE(allocatorKind, ptr);
    }
}

#endif // ifndef I_SAVANNA_MEMORY_HPP