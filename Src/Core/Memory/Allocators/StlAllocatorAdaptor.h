/**
 * @file StlAllocatorAdaptor.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-19
 *
 */
#pragma once

namespace Savanna
{
    template<typename T, Allocator TAlloc>
    class StlAllocatorAdaptor
    {
    private:
        TAlloc& m_Allocator;

    public:
        typedef T value_type;

        StlAllocatorAdaptor() = delete;

        StlAllocatorAdaptor(Allocator*)
    };
} // namespace Savanna
