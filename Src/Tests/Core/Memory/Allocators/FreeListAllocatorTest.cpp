/**
 * @file FreeListAllocatorTest.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-08
 *
 */
#include "AllocatorUnitTestWrapper.h"

#include <Types/Memory/FreeListAllocator.h>
#include <memory>

#define DECLARE_FREE_LIST_TEST_F(__name) \
    TEST_F(FreeListAllocatorTest, __name)

namespace Savanna::Core::Tests
{
    class FreeListAllocatorTest : public ::testing::Test
    {
    protected:
        AllocatorUnitTestWrapper<FreeListAllocator>* m_WrappedAllocator;

        FreeListAllocatorTest()
        {
        }

        ~FreeListAllocatorTest() override = default;

        // If the constructor and destructor are not enough for setting up
        // and cleaning up each test, you can define the following methods:
        void SetUp() override
        {
            // Code here will be called immediately after the constructor (right
            // before each test).
            m_WrappedAllocator = new AllocatorUnitTestWrapper<FreeListAllocator>(malloc(1024), 1024);
        }

        void TearDown() override
        {
            // Code here will be called immediately after each test (right
            // before the destructor).
            delete m_WrappedAllocator;
        }

        constexpr size_t GetSizeForAllocator() const { return 1024; }

        const uintptr GetRootAsUIntPtr() const
        {
            return reinterpret_cast<uintptr>(m_WrappedAllocator->GetRoot());
        }

        const int GetSize() const
        {
            return m_WrappedAllocator->GetSize();
        }
    };

    DECLARE_FREE_LIST_TEST_F(EnsureInitializationIsCorrect)
    {
        // Free list allocator starts by reserving the first chunk for the descriptor
        EXPECT_EQ(m_WrappedAllocator->GetAllocatedBytes(), sizeof(MemoryChunkDescriptor));
        EXPECT_EQ(m_WrappedAllocator->GetSize(), GetSizeForAllocator());
        EXPECT_NE(m_WrappedAllocator->GetRoot(), nullptr);
    }

    DECLARE_FREE_LIST_TEST_F(EnsureValidAllocationDoesNotReturnNullptr)
    {
        void* ptr = m_WrappedAllocator->alloc(1, 1);
        EXPECT_NE(ptr, nullptr);
    }

    DECLARE_FREE_LIST_TEST_F(EnsureInvalidAllocationReturnsNullptrOnEmptyAllocator)
    {
        void* result = nullptr;
        size_t alignment = 8;
        size_t size = 2048;
        result = m_WrappedAllocator->alloc(size, alignment, true);
        EXPECT_EQ(result, nullptr);
    }

    DECLARE_FREE_LIST_TEST_F(CheckIfAlignedAllocatePtrIsProperlyOffset)
    {
        void* result = nullptr;
        size_t alignment = 8;
        size_t size = 8;
        result = m_WrappedAllocator->alloc(size, alignment);
        EXPECT_EQ(m_WrappedAllocator->GetAllocatedBytes(), sizeof(MemoryChunkDescriptor) * 2 + size);
        EXPECT_EQ(GetRootAsUIntPtr() + sizeof(MemoryChunkDescriptor), reinterpret_cast<uintptr_t>(result));

        void* result2 = nullptr;
        result2 = m_WrappedAllocator->alloc(size, alignment);
        EXPECT_NE(result2, nullptr);
        EXPECT_EQ(m_WrappedAllocator->GetAllocatedBytes(), sizeof(MemoryChunkDescriptor) * 3 + size * 2);
    }

    DECLARE_FREE_LIST_TEST_F(DoesAllocatorProperlyRunOutOfMemoryWithAlignedPointers)
    {
        constexpr size_t expectedAllocationSize = 512 + sizeof(MemoryChunkDescriptor) * 2;
        void* result = nullptr;
        size_t alignment = 8;
        size_t size = 512;
        result = m_WrappedAllocator->alloc(size, alignment);
        EXPECT_NE(result, nullptr);
        EXPECT_EQ(
            m_WrappedAllocator->GetAllocatedBytes(), expectedAllocationSize);

        // Should fail because there is not enough memory left
        result = m_WrappedAllocator->alloc(size, alignment, true);
        EXPECT_EQ(
            m_WrappedAllocator->GetAllocatedBytes(), expectedAllocationSize);
        EXPECT_EQ(result, nullptr);
    }

    DECLARE_FREE_LIST_TEST_F(CheckIfAssertionFailsWhenAllocatingWithInvalidAlignment)
    {
        void* result = nullptr;
        size_t alignment = 0;
        size_t size = 512;
        result = m_WrappedAllocator->alloc(size, alignment, true);
        EXPECT_EQ(result, nullptr);
    }

    DECLARE_FREE_LIST_TEST_F(AllocateAndDeallocateBasicTest)
    {
        void* result = nullptr;
        size_t alignment = 8;
        size_t size = 512;
        result = m_WrappedAllocator->alloc(size, alignment);
        EXPECT_NE(result, nullptr);
        EXPECT_EQ(m_WrappedAllocator->GetAllocatedBytes(), 512 + sizeof(MemoryChunkDescriptor) * 2);

        m_WrappedAllocator->free(result, alignment);
        EXPECT_EQ(m_WrappedAllocator->GetAllocatedBytes(), sizeof(MemoryChunkDescriptor));
        EXPECT_EQ(GetRootAsUIntPtr() + sizeof(MemoryChunkDescriptor), reinterpret_cast<uintptr_t>(result));
    }

    DECLARE_FREE_LIST_TEST_F(AllocateThenDeallocateInReverseOrder)
    {
        void* result = nullptr;
        size_t alignment = 8;
        size_t size = 8;
        result = m_WrappedAllocator->alloc(size, alignment);
        EXPECT_NE(result, nullptr);
        EXPECT_EQ(m_WrappedAllocator->GetAllocatedBytes(), sizeof(MemoryChunkDescriptor) * 2 + size);
        EXPECT_EQ(GetRootAsUIntPtr() + sizeof(MemoryChunkDescriptor), reinterpret_cast<uintptr_t>(result));

        void* result2 = nullptr;
        result2 = m_WrappedAllocator->alloc(size, alignment);
        EXPECT_NE(result2, nullptr);
        EXPECT_EQ(m_WrappedAllocator->GetAllocatedBytes(), sizeof(MemoryChunkDescriptor) * 3 + size * 2);

        void* result3 = nullptr;
        result3 = m_WrappedAllocator->alloc(size, alignment);
        EXPECT_NE(result3, nullptr);
        EXPECT_EQ(m_WrappedAllocator->GetAllocatedBytes(), sizeof(MemoryChunkDescriptor) * 4 + size * 3);

        m_WrappedAllocator->free(result3, alignment);
        EXPECT_EQ(m_WrappedAllocator->GetAllocatedBytes(), sizeof(MemoryChunkDescriptor) * 3 + size * 2);

        m_WrappedAllocator->free(result2, alignment);
        EXPECT_EQ(m_WrappedAllocator->GetAllocatedBytes(), sizeof(MemoryChunkDescriptor) * 2 + size);

        m_WrappedAllocator->free(result, alignment);
        EXPECT_EQ(m_WrappedAllocator->GetAllocatedBytes(), sizeof(MemoryChunkDescriptor));
    }

    DECLARE_FREE_LIST_TEST_F(AllocateThenDeallocateInAllocationOrder)
    {
        void* result = nullptr;
        size_t alignment = 8;
        size_t size = 8;
        result = m_WrappedAllocator->alloc(size, alignment);
        EXPECT_NE(result, nullptr);
        EXPECT_EQ(m_WrappedAllocator->GetAllocatedBytes(), sizeof(MemoryChunkDescriptor) * 2 + size);
        EXPECT_EQ(GetRootAsUIntPtr() + sizeof(MemoryChunkDescriptor), reinterpret_cast<uintptr_t>(result));

        void* result2 = nullptr;
        result2 = m_WrappedAllocator->alloc(size, alignment);
        EXPECT_NE(result2, nullptr);
        EXPECT_EQ(m_WrappedAllocator->GetAllocatedBytes(), sizeof(MemoryChunkDescriptor) * 3 + size * 2);

        void* result3 = nullptr;
        result3 = m_WrappedAllocator->alloc(size, alignment);
        EXPECT_NE(result3, nullptr);
        EXPECT_EQ(m_WrappedAllocator->GetAllocatedBytes(), sizeof(MemoryChunkDescriptor) * 4 + size * 3);

        m_WrappedAllocator->free(result, alignment);
        EXPECT_EQ(m_WrappedAllocator->GetAllocatedBytes(), sizeof(MemoryChunkDescriptor) * 3 + size * 2);

        m_WrappedAllocator->free(result2, alignment);
        EXPECT_EQ(m_WrappedAllocator->GetAllocatedBytes(), sizeof(MemoryChunkDescriptor) * 2 + size);

        m_WrappedAllocator->free(result3, alignment);
        EXPECT_EQ(m_WrappedAllocator->GetAllocatedBytes(), sizeof(MemoryChunkDescriptor));
    }

    DECLARE_FREE_LIST_TEST_F(AllocateAndDeallocateAndAllocate)
    {
        void* result = nullptr;
        size_t alignment = 8;
        size_t size = 512;
        result = m_WrappedAllocator->alloc(size, alignment);
        EXPECT_NE(result, nullptr);
        EXPECT_EQ(m_WrappedAllocator->GetAllocatedBytes(), 512 + sizeof(MemoryChunkDescriptor) * 2);

        m_WrappedAllocator->free(result, alignment);
        EXPECT_EQ(m_WrappedAllocator->GetAllocatedBytes(), sizeof(MemoryChunkDescriptor));
        EXPECT_EQ(GetRootAsUIntPtr() + sizeof(MemoryChunkDescriptor), reinterpret_cast<uintptr_t>(result));

        result = m_WrappedAllocator->alloc(size, alignment);
        EXPECT_NE(result, nullptr);
        EXPECT_EQ(m_WrappedAllocator->GetAllocatedBytes(), 512 + sizeof(MemoryChunkDescriptor) * 2);

        m_WrappedAllocator->free(result, alignment);
        EXPECT_EQ(m_WrappedAllocator->GetAllocatedBytes(), sizeof(MemoryChunkDescriptor));
        EXPECT_EQ(GetRootAsUIntPtr() + sizeof(MemoryChunkDescriptor), reinterpret_cast<uintptr_t>(result));
    }
} // namespace Savanna::Tests::Memory::Allocators
