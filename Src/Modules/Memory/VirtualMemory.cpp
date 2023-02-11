#include "VirtualMemory.h"

namespace Savanna::Memory
{
    VirtualMemoryManager::VirtualMemoryManager(IVirtualMemorySpace* pVirtualMemorySpace)
        : m_upVirtualMemorySpace(pVirtualMemorySpace)
    {}

    VirtualMemoryManager::~VirtualMemoryManager()
    {
        m_upVirtualMemorySpace = nullptr;
    }

    void* VirtualMemoryManager::Allocate(size_t size, size_t alignment)
    {
        return m_upVirtualMemorySpace->Allocate(size, alignment);
    }

    void VirtualMemoryManager::Deallocate(void* ptr)
    {
        m_upVirtualMemorySpace->Deallocate(ptr);
    }

    void* VirtualMemoryManager::GetAllocatedPageCount()
    {
        return m_upVirtualMemorySpace->GetAllocatedPageCount();
    }

    size_t VirtualMemoryManager::GetPageSize()
    {
        return m_upVirtualMemorySpace->GetPageSize();
    }
}
