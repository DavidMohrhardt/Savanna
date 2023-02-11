/**
 * @file VirtualMemory.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-02-03
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <SavannaEngine.h>
#include <Utilities/SavannaCoding.h>

#include <Allocator.h>
#include <CacheLine.h>
#include <MemoryBlocks.h>
#include <MemoryBuffer.h>
#include <MemoryChunkDescriptors.h>

#include <Types/Containers/Arrays/ArraySlice.h>

#include <vector>

namespace Savanna
{
    class IVirtualMemorySpace
    {
    protected:
        IVirtualMemorySpace() = default;
        virtual ~IVirtualMemorySpace() = default;

    public:
        virtual void* Allocate(size_t size, size_t alignment) = 0;
        virtual void Deallocate(void* ptr) = 0;
        virtual size_t GetAllocatedPageCount() = 0;
        virtual size_t GetPageSize() = 0;
    };

    template <typename PAGE, size SIZE>
    class PageVirtualMemorySpace : public IVirtualMemorySpace
    {
        constexpr size k_PageSize = SIZE;
    private:
        std::unordered_map<int32_t, std::unique_ptr<PAGE>> m_PageTable;
        std::vector<std::unique_ptr<PAGE>> m_PagePool;

    public:
        PageVirtualMemorySpace()
        {
            m_PagePool.reserve(100);
        }

        void* Allocate(size_t size, size_t alignment) override
        {
            if (size > k_PageSize)
            {
                return nullptr;
            }

            if (m_PagePool.empty())
            {
                PAGE* page = static<PAGE*>(malloc(k_PageSize));
                m_PagePool.emplace_back(std::make_unique<PAGE>(page));
            }

            auto pPage = m_PagePool.back().release();
            m_PagePool.pop_back();

            return pPage;
        }

        void Deallocate(void* ptr) override
        {
            m_PagePool.emplace_back(std::make_unique<PAGE>(reinterpret_cast<PAGE*>(ptr)));
        }

        size_t GetAllocatedPageCount() override
        {
            return m_PagePool.size();
        }

        size_t GetPageSize() override
        {
            return k_PageSize;
        }
    };

    class VirtualMemoryManager SAVANNA_FINAL : private Singleton<VirtualMemoryManager>
    {
    public:
        template <typename T, typename... Args>
        requires std::derived_from<T, IVirtualMemorySpace>
        static void Construct(Args... args)
        {
            auto pVirtualMemorySpace = new T(std::forward<Args>(args)...);
            Singleton<VirtualMemoryManager>::Construct(pVirtualMemorySpace);
        }

        static void Destroy()
        {
            Singleton<VirtualMemoryManager>::Destroy();
            g_VirtualMemorySpace = nullptr;
        }

        static VirtualMemoryManager& GetInstance()
        {
            return Singleton<VirtualMemoryManager>::GetInstance();
        }
    private:
        std::unique_ptr<IVirtualMemorySpace> m_upVirtualMemorySpace = nullptr;
        VirtualMemoryManager(IVirtualMemorySpace* pVirtualMemorySpace);

    public:
        ~VirtualMemoryManager();

        void* Allocate(size_t size, size_t alignment);
        void Deallocate(void* ptr);
        void* GetAllocatedPageCount();
        size_t GetPageSize();
    };
} // namespace Savanna
