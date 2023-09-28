#include "VkAllocator.h"

namespace Savanna::Gfx::Vk2
{
    void* SavannaGfxVk2AllocCallback(
        void *pUserData, size_t size, size_t alignment, VkSystemAllocationScope allocationScope);
    void* SavannaGfxVk2ReallocCallback(
        void *pUserData, void *pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope);
    void SavannaGfxVk2FreeCallback(
        void *pUserData, void *pMemory);

    static VkAllocationCallbacks k_DefaultVkAllocationCallbacks =
    {
        nullptr, // void* pUserData;
        SavannaGfxVk2AllocCallback, // PFN_vkAllocationFunction pfnAllocation;
        SavannaGfxVk2ReallocCallback, // PFN_vkReallocationFunction pfnReallocation;
        SavannaGfxVk2FreeCallback, // PFN_vkFreeFunction pfnFree;
        nullptr, // PFN_vkInternalAllocationNotification pfnInternalAllocation;
        nullptr, // PFN_vkInternalFreeNotification pfnInternalFree;
    };

    void* SavannaGfxVk2AllocCallback(void *pUserData, size_t size, size_t alignment, VkSystemAllocationScope allocationScope)
    {
        VkAllocator* pVkAllocator = reinterpret_cast<VkAllocator*>(pUserData);
        if (pVkAllocator == nullptr)
        {
            throw std::runtime_error("pUserData is nullptr!");
        }
        return pVkAllocator->m_Allocator.AllocateAligned(size, alignment);
    }

    void* SavannaGfxVk2ReallocCallback(void *pUserData, void *pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope)
    {
        VkAllocator* pVkAllocator = reinterpret_cast<VkAllocator*>(pUserData);
        if (pVkAllocator == nullptr)
        {
            throw std::runtime_error("pUserData is nullptr!");
        }
        return pVkAllocator->m_Allocator.ReallocateAligned(pOriginal, size, alignment);
    }

    void SavannaGfxVk2FreeCallback(void *pUserData, void *pMemory)
    {
        VkAllocator* pVkAllocator = reinterpret_cast<VkAllocator*>(pUserData);
        if (pVkAllocator == nullptr)
        {
            throw std::runtime_error("pUserData is nullptr!");
        }
        pVkAllocator->m_Allocator.Free(pMemory);
    }

    VkAllocationCallbacks GetVkAllocationCallbacks(VkAllocator* pRealAllocator)
    {
        VkAllocationCallbacks allocationCallbacks = k_DefaultVkAllocationCallbacks;
        allocationCallbacks.pUserData = (void*)pRealAllocator;
        return allocationCallbacks;
    }
} // namespace Savanna::Gfx::Vk2
