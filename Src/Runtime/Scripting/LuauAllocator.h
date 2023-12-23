#pragma once

#include <Memory/SavannaMemory.h>

#include <lua.h>
#include <luau.h>


namespace savanna::luau
{
    namespace
    {
        inline void* alloc(void* ud, void* ptr, size_t osize, size_t nsize)
        {
            if (nsize == 0)
            {
                SAVANNA_FREE(kSavannaAllocatorKindGeneral, ptr);
                return NULL;
            }
            else
            {
                return SAVANNA_REALLOC(kSavannaAllocatorKindGeneral, ptr, nsize);
            }
        }
    } // namespace
} // namespace savanna::luau
