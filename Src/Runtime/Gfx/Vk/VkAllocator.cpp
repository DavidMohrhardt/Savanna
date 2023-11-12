#include "VkAllocator.h"

namespace Savanna::Gfx::Vk2
{
    static se_AllocatorInterface_t s_AllocationInterface {};
    static VkAllocationCallbacks* s_pAllocationCallbacks = nullptr;

    VkAllocationCallbacks VkAllocator::s_AllocationCallbacks {
        .pUserData = &s_AllocationInterface,
        .pfnAllocation = Alloc,
        .pfnReallocation = Realloc,
        .pfnFree = Free,
        .pfnInternalAllocation = nullptr,
        .pfnInternalFree = nullptr
    };

    void* VkAllocator::Alloc(void *pUserData, size_t size, size_t alignment, VkSystemAllocationScope allocationScope)
    {
#if !SAVANNA_ENABLE_VK_ALLOCATOR_TRACKING
        return SAVANNA_INTERFACE_ALLOCATE_ALIGNED(pUserData, size, alignment, nullptr);
#else
        void* pBuffer = SAVANNA_INTERFACE_ALLOCATE_ALIGNED(pUserData, size, alignment, nullptr);
        if (pBuffer != nullptr)
        {
            s_Allocations++;
            s_AllocationsMax = std::max(s_AllocationsMax, s_Allocations);
        }
        return pBuffer;
#endif // !SAVANNA_ENABLE_VK_ALLOCATOR_TRACKING
    }

    void* VkAllocator::Realloc(void *pUserData, void *pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope)
    {
        void* newBuffer = SAVANNA_INTERFACE_ALLOCATE_ALIGNED(pUserData, size, alignment, nullptr);
        if (pOriginal != nullptr)
        {
            ::memcpy(newBuffer, pOriginal, size);
            SAVANNA_INTERFACE_FREE(pUserData, pOriginal, nullptr);
        }
        return newBuffer;
    }

    void VkAllocator::Free(void *pUserData, void *pMemory)
    {
        if (pMemory == nullptr)
        {
            return;
        }

#if SAVANNA_ENABLE_VK_ALLOCATOR_TRACKING
        s_Allocations -= 1;
#endif // SAVANNA_ENABLE_VK_ALLOCATOR_TRACKING

        SAVANNA_INTERFACE_FREE(pUserData, pMemory, nullptr);
    }

    const VkAllocationCallbacks *const VkAllocator::Get()
    {
        return s_pAllocationCallbacks;
    }

    const se_AllocatorInterface_t &VkAllocator::GetAllocatorInterface()
    {
        return s_AllocationInterface;
    }

    void VkAllocator::SetVkAllocationInterfacePtr(const se_AllocatorInterface_t* pInterface)
    {
        if (pInterface != nullptr)
        {
            s_AllocationInterface = *pInterface;
            s_AllocationCallbacks.pUserData = &s_AllocationInterface;
            s_pAllocationCallbacks = &s_AllocationCallbacks;
        }
        else
        {
#if SAVANNA_ENABLE_VK_ALLOCATOR_TRACKING
            if (s_Allocations != 0)
            {
                SAVANNA_FATAL_LOG("Vulkan allocator is being destroyed with {} allocations still active!", s_Allocations);
            }
#endif // SAVANNA_ENABLE_VK_ALLOCATOR_TRACKING
            s_AllocationInterface = {};
            s_pAllocationCallbacks = nullptr;
        }
    }
} // namespace Savanna::Gfx::Vk2
