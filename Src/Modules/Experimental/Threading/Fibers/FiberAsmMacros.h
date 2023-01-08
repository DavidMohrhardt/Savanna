#pragma once
#include "SavannaEngine.h"

#include "PlatformDefinitions.h"

#define ASM_SET_FIBER_STATE_ACTIVE \
    static_assert(false, "No proper definition for 'stackful' fibers.  Is your architecture supported?")

#define ASM_SAVANNA_FIBER_YIELD \
    static_assert(false, "No proper definition for 'stackful' fibers.  Is your architecture supported?")

#define ASM_SET_FIBER_STATE_COMPLETE \
    static_assert(false, "No proper definition for 'stackful' fibers.  Is your architecture supported?")

// First check compiler
#if SAVANNA_COMPILER_CLANG
#if SAVANNA_ARCH_AMDX64

#endif // SAVANNA_ARCH_AMDX64
#elif SAVANNA_COMPILER_MSVC
#if SAVANNA_ARCH_AMDX64
#undef ASM_SET_FIBER_STATE_ACTIVE
#define ASM_SET_FIBER_STATE_ACTIVE  \
    __asm                           \
    {                               \
                                    \
    }

#undef ASM_SET_FIBER_STATE_COMPLETE
#define ASM_SET_FIBER_STATE_COMPLETE \
    static_assert(false, "No proper definition for 'stackful' fibers.  Is your architecture supported?")

#elif SAVANNA_ARCH_X64

#undef ASM_SET_FIBER_STATE_ACTIVE
#define ASM_SET_FIBER_STATE_ACTIVE  \
    __asm                           \
    {                               \
                                    \
    }

__asm
{
    mov m_PreviousBasePtr, RBP
    // Save the current stack pointer
    mov m_StackOffsetPtr, RSP
    // Replace stack pointer with allocated address
    mov RBP, m_StackBasePtr
    // Add offset to StackPtr
    mov eax, dword m_AllocatedStackPtr[0]
    addi rsp, eax, rsp
    // Setup a register to read the most recent instruction address
    mov eax, dword m_AllocatedStackPtr[1]
    // Jump to the address to return to processing.
    jmp eax
}

// Post Yield
__asm
{
    // Reload the registers state
    POPA
    //
}

// Yield
__asm
{
    // PUSH the registers
    PUSHA
    // set up the registers to load the IP + offset
}

// inline void VectorAdd(Vector* res, Vector* lhs, Vector* rhs) {
//     __asm {
//         mov eax, lhs
//         movups xmm1, [eax]
//         mov eax, rhs
//         movups xmm0, [eax]
//         addps  xmm1, xmm0
//         mov eax, res
//         movups[eax], xmm1
//         emms
//     }
// }

#undef ASM_SET_FIBER_STATE_COMPLETE
#define ASM_SET_FIBER_STATE_COMPLETE \
    static_assert(false, "No proper definition for 'stackful' fibers.  Is your architecture supported?")

#endif // SAVANNA_ARCH_AMDX64
#elif SAVANNA_COMPILER_GCC

// TODO @davidmo

#endif
