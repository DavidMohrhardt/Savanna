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

    template <size POOL_SIZE, size SIZE>
    class VirtualPageMemorySpace : public IVirtualMemorySpace
    {
    private:
        friend class VirtualMemoryManager;

        constexpr size k_PageSize = SIZE;

        struct Page
        {
            byte m_Data[SIZE];
        };

    private:
        Page m_PagePool[POOL_SIZE];

    private:
        VirtualPageMemorySpace() {}

    public:
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

        const size_t GetAllocatedPageCount() const override
        {
            return m_PagePool.size();
        }

        const inline size_t GetPageSize() const override
        {
            return k_PageSize;
        }
    };

    class VirtualMemoryManager SAVANNA_FINAL : private Singleton<VirtualMemoryManager>
    {
    public:
        template <typename T, typename... Args>
        requires std::derived_from<T, IVirtualMemorySpace>
        inline void InitializeIVirtualMemorySpace(Args... args)
        {
            m_upVirtualMemorySpace = std::make_unique<T>(std::forward<Args>(args)...);
        }

    private:
        std::unique_ptr<IVirtualMemorySpace> m_upVirtualMemorySpace = nullptr;

    public:
        VirtualMemoryManager();
        ~VirtualMemoryManager();

        void* Allocate(size_t size, size_t alignment);
        void Deallocate(void* ptr);
        void* GetAllocatedPageCount();
        inline const size_t GetPageSize() const { return m_upVirtualMemorySpace->GetPageSize(); }
    };
} // namespace Savanna
