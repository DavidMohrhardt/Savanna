/**
 * @file FreeListAllocatorTest.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-08
 *
 */
#include "AllocatorTestUtils.h"

#include <Memory/MemoryArena.h>
#include <Memory/Allocators/FreeListAllocator.h>
#include <memory>

#define DECLARE_FREE_LIST_TEST(__name) \
    DECLARE_ALLOCATOR_TEST(FreeList, __name)

#define DECLARE_FREE_LIST_TEST_F(__name) \
    TEST_F(FreeListAllocatorTest, __name)

namespace Savanna::Core::Tests
{
    class FreeListAllocatorTest : public ::testing::Test
    {
    protected:
        FreeListAllocatorTest()
        {
            m_Root = malloc(1024);
            m_Size = 1024;
            m_Allocator = FreeListAllocator(m_Root, m_Size);
        }

        ~FreeListAllocatorTest() override
        {
            if (m_Root != nullptr)
            {
                free(m_Root);
            }
        }

        // If the constructor and destructor are not enough for setting up
        // and cleaning up each test, you can define the following methods:

        void SetUp() override
        {
            // For Universal Setup
        }

        void TearDown() override
        {
            // Code here will be called immediately after each test (right
            // before the destructor).
        }

        const uintptr_t GetRootAsUIntPtr() const
        {
            return reinterpret_cast<uintptr_t>(m_Root);
        }

        const int GetSize() const
        {
            return m_Size;
        }

        void Allocate(size_t size, size_t alignment, void** out_ptr)
        {
            *out_ptr = m_Allocator.Allocate(size, alignment);
        }

        void Deallocate(void* ptr, const size_t alignment)
        {
            m_Allocator.Deallocate(ptr, alignment);
        }

    private:
        void* m_Root;
        int m_Size;

    protected:
        FreeListAllocator m_Allocator;
    };

    DECLARE_FREE_LIST_TEST_F(EnsureInitializationIsCorrect)
    {
        EXPECT_EQ(m_Allocator.GetSize(), GetSize());
        EXPECT_EQ(m_Allocator.GetAllocatedBytes(), sizeof(MemoryChunkHeader));
        EXPECT_EQ(m_Allocator.GetSize(), GetSize());
    }

    DECLARE_FREE_LIST_TEST_F(EnsureValidAllocationDoesNotReturnNullptr)
    {
        void* ptr = nullptr;
        Allocate(1, 1, &ptr);
        EXPECT_NE(ptr, nullptr);
    }

    DECLARE_FREE_LIST_TEST_F(EnsureInvalidAllocationReturnsNullptrOnEmptyAllocator)
    {
        void* result = nullptr;
        size_t alignment = 8;
        size_t size = 2048;
        Allocate(size, alignment, &result);
        EXPECT_EQ(result, nullptr);
    }

    DECLARE_FREE_LIST_TEST_F(CheckIfAlignedAllocatePtrIsProperlyOffset)
    {
        void* result = nullptr;
        size_t alignment = 8;
        size_t size = 8;
        EXPECT_NO_THROW(Allocate(size, alignment, &result));
        EXPECT_NE(result, nullptr);
        EXPECT_EQ(m_Allocator.GetAllocatedBytes(), sizeof(MemoryChunkHeader) * 2 + size);
        EXPECT_EQ(GetRootAsUIntPtr() + sizeof(MemoryChunkHeader), reinterpret_cast<uintptr_t>(result));

        void* result2 = nullptr;
        EXPECT_NO_THROW(Allocate(size, alignment, &result2));
        EXPECT_NE(result2, nullptr);
        EXPECT_EQ(m_Allocator.GetAllocatedBytes(), sizeof(MemoryChunkHeader) * 3 + size * 2);
    }

    DECLARE_FREE_LIST_TEST_F(DoesAllocatorProperlyRunOutOfMemoryWithAlignedPointers)
    {
        void* result = nullptr;
        size_t alignment = 8;
        size_t size = 512;
        EXPECT_NO_THROW(Allocate(size, alignment, &result));
        EXPECT_NE(result, nullptr);
        EXPECT_EQ(m_Allocator.GetAllocatedBytes(), 512 + sizeof(MemoryChunkHeader) * 2);

        EXPECT_NO_THROW(Allocate(size, alignment, &result));
        EXPECT_EQ(m_Allocator.GetAllocatedBytes(), 512 + sizeof(MemoryChunkHeader) * 2);
        EXPECT_EQ(result, nullptr);
    }

    DECLARE_FREE_LIST_TEST_F(CheckIfAssertionFailsWhenAllocatingWithInvalidAlignment)
    {
        void* result = nullptr;
        size_t alignment = 0;
        size_t size = 512;
        EXPECT_NO_THROW(Allocate(size, alignment, &result));
        EXPECT_EQ(result, nullptr);
    }

    DECLARE_FREE_LIST_TEST_F(AllocateAndDeallocateBasicTest)
    {
        void* result = nullptr;
        size_t alignment = 8;
        size_t size = 512;
        EXPECT_NO_THROW(Allocate(size, alignment, &result));
        EXPECT_NE(result, nullptr);
        EXPECT_EQ(m_Allocator.GetAllocatedBytes(), 512 + sizeof(MemoryChunkHeader) * 2);

        EXPECT_NO_THROW(Deallocate(result, alignment));
        EXPECT_EQ(m_Allocator.GetAllocatedBytes(), sizeof(MemoryChunkHeader));
        EXPECT_EQ(GetRootAsUIntPtr() + sizeof(MemoryChunkHeader), reinterpret_cast<uintptr_t>(result));
    }

    DECLARE_FREE_LIST_TEST_F(AllocateThenDeallocateInReverseOrder)
    {
        void* result = nullptr;
        size_t alignment = 8;
        size_t size = 8;
        EXPECT_NO_THROW(Allocate(size, alignment, &result));
        EXPECT_NE(result, nullptr);
        EXPECT_EQ(m_Allocator.GetAllocatedBytes(), sizeof(MemoryChunkHeader) * 2 + size);
        EXPECT_EQ(GetRootAsUIntPtr() + sizeof(MemoryChunkHeader), reinterpret_cast<uintptr_t>(result));

        void* result2 = nullptr;
        EXPECT_NO_THROW(Allocate(size, alignment, &result2));
        EXPECT_NE(result2, nullptr);
        EXPECT_EQ(m_Allocator.GetAllocatedBytes(), sizeof(MemoryChunkHeader) * 3 + size * 2);

        void* result3 = nullptr;
        EXPECT_NO_THROW(Allocate(size, alignment, &result3));
        EXPECT_NE(result3, nullptr);
        EXPECT_EQ(m_Allocator.GetAllocatedBytes(), sizeof(MemoryChunkHeader) * 4 + size * 3);

        EXPECT_NO_THROW(Deallocate(result3, alignment));
        EXPECT_EQ(m_Allocator.GetAllocatedBytes(), sizeof(MemoryChunkHeader) * 3 + size * 2);

        EXPECT_NO_THROW(Deallocate(result2, alignment));
        EXPECT_EQ(m_Allocator.GetAllocatedBytes(), sizeof(MemoryChunkHeader) * 2 + size);

        EXPECT_NO_THROW(Deallocate(result, alignment));
        EXPECT_EQ(m_Allocator.GetAllocatedBytes(), sizeof(MemoryChunkHeader));
    }

    // Test failing
    DECLARE_FREE_LIST_TEST_F(AllocateThenDeallocateInAllocationOrder)
    {
        void* result = nullptr;
        size_t alignment = 8;
        size_t size = 8;
        EXPECT_NO_THROW(Allocate(size, alignment, &result));
        EXPECT_NE(result, nullptr);
        EXPECT_EQ(m_Allocator.GetAllocatedBytes(), sizeof(MemoryChunkHeader) * 2 + size);
        EXPECT_EQ(GetRootAsUIntPtr() + sizeof(MemoryChunkHeader), reinterpret_cast<uintptr_t>(result));

        void* result2 = nullptr;
        EXPECT_NO_THROW(Allocate(size, alignment, &result2));
        EXPECT_NE(result2, nullptr);
        EXPECT_EQ(m_Allocator.GetAllocatedBytes(), sizeof(MemoryChunkHeader) * 3 + size * 2);

        void* result3 = nullptr;
        EXPECT_NO_THROW(Allocate(size, alignment, &result3));
        EXPECT_NE(result3, nullptr);
        EXPECT_EQ(m_Allocator.GetAllocatedBytes(), sizeof(MemoryChunkHeader) * 4 + size * 3);

        EXPECT_NO_THROW(Deallocate(result, alignment));
        EXPECT_EQ(m_Allocator.GetAllocatedBytes(), sizeof(MemoryChunkHeader) * 3 + size * 2);

        EXPECT_NO_THROW(Deallocate(result2, alignment));
        EXPECT_EQ(m_Allocator.GetAllocatedBytes(), sizeof(MemoryChunkHeader) * 2 + size);

        EXPECT_NO_THROW(Deallocate(result3, alignment));
        EXPECT_EQ(m_Allocator.GetAllocatedBytes(), sizeof(MemoryChunkHeader));
    }

    DECLARE_FREE_LIST_TEST_F(AllocateAndDeallocateAndAllocate)
    {
        void* result = nullptr;
        size_t alignment = 8;
        size_t size = 512;
        EXPECT_NO_THROW(Allocate(size, alignment, &result));
        EXPECT_NE(result, nullptr);
        EXPECT_EQ(m_Allocator.GetAllocatedBytes(), 512 + sizeof(MemoryChunkHeader) * 2);

        EXPECT_NO_THROW(Deallocate(result, alignment));
        EXPECT_EQ(m_Allocator.GetAllocatedBytes(), sizeof(MemoryChunkHeader));
        EXPECT_EQ(GetRootAsUIntPtr() + sizeof(MemoryChunkHeader), reinterpret_cast<uintptr_t>(result));

        EXPECT_NO_THROW(Allocate(size, alignment, &result));
        EXPECT_NE(result, nullptr);
        EXPECT_EQ(m_Allocator.GetAllocatedBytes(), 512 + sizeof(MemoryChunkHeader) * 2);

        EXPECT_NO_THROW(Deallocate(result, alignment));
        EXPECT_EQ(m_Allocator.GetAllocatedBytes(), sizeof(MemoryChunkHeader));
        EXPECT_EQ(GetRootAsUIntPtr() + sizeof(MemoryChunkHeader), reinterpret_cast<uintptr_t>(result));
    }
} // namespace Savanna::Tests::Memory::Allocators
