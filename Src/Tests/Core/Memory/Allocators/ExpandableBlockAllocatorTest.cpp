/**
 * @file ExpandableBlockAllocatorTest.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-08
 *
 */
#include "AllocatorTestUtils.h"

#include <Types/Memory/ExpandableBlockAllocator.h>
#include <memory>

#define DECLARE_EXPANDABLE_BLOCK_TEST(__name) \
    DECLARE_ALLOCATOR_TEST(ExpandableBlock, __name)

#define DECLARE_EXPANDABLE_BLOCK_TEST_F(__name) \
    TEST_F(ExpandableBlockAllocatorTest, __name)

namespace Savanna::Core::Tests
{

    class ExpandableBlockAllocatorTest : public ::testing::Test
    {
    protected:
        ExpandableBlockAllocatorTest()
            : m_Allocator(1, 1024)
        {
        }

        ~ExpandableBlockAllocatorTest() override {}

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

        void Allocate(size_t size, size_t alignment, void** out_ptr)
        {
            *out_ptr = m_Allocator.alloc(size, alignment);
        }

        void Deallocate(void* ptr, const size_t alignment)
        {
            m_Allocator.free(ptr, alignment);
        }

    protected:
        ExpandableBlockAllocator m_Allocator;
    };

    DECLARE_EXPANDABLE_BLOCK_TEST_F(EnsureValidAllocationDoesNotReturnNullptr)
    {
        void* ptr = nullptr;
        Allocate(1, 1, &ptr);
        EXPECT_NE(ptr, nullptr);
    }

    DECLARE_EXPANDABLE_BLOCK_TEST_F(EnsureAllocatorReturnsValidPtrEvenWhenRequestingLargerBlockSizes)
    {
        void* result = nullptr;
        size_t alignment = 8;
        size_t size = 2048;
        Allocate(size, alignment, &result);
        EXPECT_NE(result, nullptr);
    }

    DECLARE_EXPANDABLE_BLOCK_TEST_F(CheckIfAlignedAllocatePtrIsProperlyOffset)
    {
        void* result = nullptr;
        size_t alignment = 8;
        size_t size = 8;
        EXPECT_NO_THROW(Allocate(size, alignment, &result));
        EXPECT_NE(result, nullptr);
        EXPECT_EQ(m_Allocator.GetAllocatedBytes(), sizeof(MemoryChunkDescriptor) * 3 + size);

        void* result2 = nullptr;
        EXPECT_NO_THROW(Allocate(size, alignment, &result2));
        EXPECT_NE(result2, nullptr);
        EXPECT_EQ(m_Allocator.GetAllocatedBytes(), sizeof(MemoryChunkDescriptor) * 4 + size * 2);
    }

    DECLARE_EXPANDABLE_BLOCK_TEST_F(AllocateAndDeallocateBasicTest)
    {
        void* result = nullptr;
        size_t alignment = 8;
        size_t size = 512;
        EXPECT_NO_THROW(Allocate(size, alignment, &result));
        EXPECT_NE(result, nullptr);
        EXPECT_EQ(m_Allocator.GetAllocatedBytes(), 512 + sizeof(MemoryChunkDescriptor) * 3);

        EXPECT_NO_THROW(Deallocate(result, alignment));
        EXPECT_EQ(m_Allocator.GetAllocatedBytes(), sizeof(MemoryChunkDescriptor) * 2);
    }

    // DECLARE_EXPANDABLE_BLOCK_TEST_F(AllocateThenDeallocateInReverseOrder)
    // {
    //     void* result = nullptr;
    //     size_t alignment = 8;
    //     size_t size = 8;
    //     EXPECT_NO_THROW(Allocate(size, alignment, &result));
    //     EXPECT_NE(result, nullptr);
    //     EXPECT_EQ(m_Allocator.GetAllocatedBytes(), sizeof(MemoryChunkDescriptor) * 2 + size);

    //     void* result2 = nullptr;
    //     EXPECT_NO_THROW(Allocate(size, alignment, &result2));
    //     EXPECT_NE(result2, nullptr);
    //     EXPECT_EQ(m_Allocator.GetAllocatedBytes(), sizeof(MemoryChunkDescriptor) * 3 + size * 2);

    //     void* result3 = nullptr;
    //     EXPECT_NO_THROW(Allocate(size, alignment, &result3));
    //     EXPECT_NE(result3, nullptr);
    //     EXPECT_EQ(m_Allocator.GetAllocatedBytes(), sizeof(MemoryChunkDescriptor) * 4 + size * 3);

    //     EXPECT_NO_THROW(Deallocate(result3, alignment));
    //     EXPECT_EQ(m_Allocator.GetAllocatedBytes(), sizeof(MemoryChunkDescriptor) * 3 + size * 2);

    //     EXPECT_NO_THROW(Deallocate(result2, alignment));
    //     EXPECT_EQ(m_Allocator.GetAllocatedBytes(), sizeof(MemoryChunkDescriptor) * 2 + size);

    //     EXPECT_NO_THROW(Deallocate(result, alignment));
    //     EXPECT_EQ(m_Allocator.GetAllocatedBytes(), sizeof(MemoryChunkDescriptor));
    // }

    // DECLARE_EXPANDABLE_BLOCK_TEST_F(AllocateThenDeallocateInAllocationOrder)
    // {
    //     void* result = nullptr;
    //     size_t alignment = 8;
    //     size_t size = 8;
    //     EXPECT_NO_THROW(Allocate(size, alignment, &result));
    //     EXPECT_NE(result, nullptr);
    //     EXPECT_EQ(m_Allocator.GetAllocatedBytes(), sizeof(MemoryChunkDescriptor) * 2 + size);

    //     void* result2 = nullptr;
    //     EXPECT_NO_THROW(Allocate(size, alignment, &result2));
    //     EXPECT_NE(result2, nullptr);
    //     EXPECT_EQ(m_Allocator.GetAllocatedBytes(), sizeof(MemoryChunkDescriptor) * 3 + size * 2);

    //     void* result3 = nullptr;
    //     EXPECT_NO_THROW(Allocate(size, alignment, &result3));
    //     EXPECT_NE(result3, nullptr);
    //     EXPECT_EQ(m_Allocator.GetAllocatedBytes(), sizeof(MemoryChunkDescriptor) * 4 + size * 3);

    //     EXPECT_NO_THROW(Deallocate(result, alignment));
    //     EXPECT_EQ(m_Allocator.GetAllocatedBytes(), sizeof(MemoryChunkDescriptor) * 3 + size * 2);

    //     EXPECT_NO_THROW(Deallocate(result2, alignment));
    //     EXPECT_EQ(m_Allocator.GetAllocatedBytes(), sizeof(MemoryChunkDescriptor) * 2 + size);

    //     EXPECT_NO_THROW(Deallocate(result3, alignment));
    //     EXPECT_EQ(m_Allocator.GetAllocatedBytes(), sizeof(MemoryChunkDescriptor));
    // }

    // DECLARE_EXPANDABLE_BLOCK_TEST_F(AllocateAndDeallocateAndAllocate)
    // {
    //     void* result = nullptr;
    //     size_t alignment = 8;
    //     size_t size = 512;
    //     EXPECT_NO_THROW(Allocate(size, alignment, &result));
    //     EXPECT_NE(result, nullptr);
    //     EXPECT_EQ(m_Allocator.GetAllocatedBytes(), 512 + sizeof(MemoryChunkDescriptor) * 2);

    //     EXPECT_NO_THROW(Deallocate(result, alignment));
    //     EXPECT_EQ(m_Allocator.GetAllocatedBytes(), sizeof(MemoryChunkDescriptor));

    //     EXPECT_NO_THROW(Allocate(size, alignment, &result));
    //     EXPECT_NE(result, nullptr);
    //     EXPECT_EQ(m_Allocator.GetAllocatedBytes(), 512 + sizeof(MemoryChunkDescriptor) * 2);

    //     EXPECT_NO_THROW(Deallocate(result, alignment));
    //     EXPECT_EQ(m_Allocator.GetAllocatedBytes(), sizeof(MemoryChunkDescriptor));
    // }
} // namespace Savanna::Tests::Memory::Allocators
