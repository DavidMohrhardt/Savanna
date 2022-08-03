#pragma once

// For debugging purposes
#define __USE_OS_FIBERS__ 0

#include "SavannaEngine.h"

#include <cstdlib>

// This is a very os dependent functionality, as such interacting
// with this class will extend down to os specific functions no std::lib

#if SAVANNA_WINDOWS && __USE_OS_FIBERS__
#include <Windows.h>
namespace Savanna
{
    // Leverage windows fiber system
    //static_assert(false, "Windows Fiber class not yet implemented...");

    struct Fiber : Win::Fiber
}

#else
#include "FiberAsmMacros.h"

// Use portable fibers
namespace Savanna
{
    struct alignas(8) FiberAllocationInfo
    {
        uint32 m_TotalAllocationSize;
        uint16 m_StackHeadOffset;
        uint16 m_RegisterHeadOffset;
        byte *m_MemoryBlock;
    };

    struct alignas(16) Fiber
    {
        FiberAllocationInfo m_AllocationInfo;
        void (*m_FuncPtr)();

        virtual void Execute()
        {
            /*ASM_SET_FIBER_STATE_ACTIVE;
            m_FuncPtr();
            ASM_SET_FIBER_STATE_COMPLETE; */
        }

        // End goal is to use an allocator that
        // grabs from execution memory space
        // and is capable of allocating a requested amount of memory
        // as the stack.
        void* operator new(size_t size)
        {
            size_t fullAllocationSize = size + 256;
            void* m_Fiber = calloc(1, fullAllocationSize);
            reinterpret_cast<Fiber*>(m_Fiber)->m_AllocationInfo = {
                fullAllocationSize,
                (uint16)(size + sizeof(Fiber)),
                (uint16)(size + sizeof(Fiber)),
                reinterpret_cast<byte*>(&(reinterpret_cast<Fiber*>(m_Fiber)->m_FuncPtr) + sizeof(intptr_t))
            };
        }

        void operator delete(void* ptr)
        {
            free(ptr);
        }
    };
}

#endif // SAVANNA_WINDOWS