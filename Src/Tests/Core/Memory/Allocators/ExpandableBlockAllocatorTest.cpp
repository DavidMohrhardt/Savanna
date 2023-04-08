/**
 * @file ExpandableBlockAllocatorTest.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.0.1
 * @date 2022-08-08
 *
 */
#include "AllocatorUnitTestWrapper.h"

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
        AllocatorUnitTestWrapper<ExpandableBlockAllocator>* m_WrappedAllocator;

        ExpandableBlockAllocatorTest()
        {
        }

        ~ExpandableBlockAllocatorTest() override = default;

        // If the constructor and destructor are not enough for setting up
        // and cleaning up each test, you can define the following methods:
        void SetUp() override
        {
            // Code here will be called immediately after the constructor (right
            // before each test).
            m_WrappedAllocator = new AllocatorUnitTestWrapper<ExpandableBlockAllocator>(1, 1024);
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

    DECLARE_EXPANDABLE_BLOCK_TEST_F(EnsureValidAllocationDoesNotReturnNullptr)
    {
        void* ptr = nullptr;
        ptr = m_WrappedAllocator->alloc(1, 1);
        EXPECT_NE(ptr, nullptr);
    }

    DECLARE_EXPANDABLE_BLOCK_TEST_F(EnsureAllocatorReturnsValidPtrEvenWhenRequestingLargerBlockSizes)
    {
        void* result = nullptr;
        size_t alignment = 8;
        size_t size = 2048;
        result = m_WrappedAllocator->alloc(size, alignment);
        EXPECT_NE(result, nullptr);
    }

    DECLARE_EXPANDABLE_BLOCK_TEST_F(CheckIfAlignedAllocatePtrIsProperlyOffset)
    {
        void* result = nullptr;
        size_t alignment = 8;
        size_t size = 8;
        result = m_WrappedAllocator->alloc(size, alignment);
        EXPECT_NE(result, nullptr);
        EXPECT_EQ(m_WrappedAllocator->GetAllocatedBytes(), sizeof(MemoryChunkDescriptor) * 3 + size);

        void* result2 = nullptr;
        result2 = m_WrappedAllocator->alloc(size, alignment);
        EXPECT_NE(result2, nullptr);
        EXPECT_EQ(m_WrappedAllocator->GetAllocatedBytes(), sizeof(MemoryChunkDescriptor) * 4 + size * 2);
    }

    DECLARE_EXPANDABLE_BLOCK_TEST_F(AllocateAndDeallocateBasicTest)
    {
        void* result = nullptr;
        size_t alignment = 8;
        size_t size = 512;
        result = m_WrappedAllocator->alloc(size, alignment);
        EXPECT_NE(result, nullptr);
        EXPECT_EQ(m_WrappedAllocator->GetAllocatedBytes(), size + sizeof(MemoryChunkDescriptor) * 3);

        m_WrappedAllocator->free(result, alignment);
        EXPECT_EQ(m_WrappedAllocator->GetAllocatedBytes(), sizeof(MemoryChunkDescriptor) * 2);
    }

    DECLARE_EXPANDABLE_BLOCK_TEST_F(AllocateThenDeallocateInReverseOrder)
    {
        constexpr size_t size = 8;
        constexpr size_t alignment = 8;
        constexpr uint32 initialMemoryChunkDescCount = 2;

        constexpr size_t sizeOfOneAllocation = size + sizeof(MemoryChunkDescriptor);
        size_t totalAllocations = m_WrappedAllocator->GetSize() / sizeOfOneAllocation;

        size_t originalSize = m_WrappedAllocator->GetSize();

        void** ptrs = new void*[totalAllocations];
        for (uint32 i = 0; i < totalAllocations; ++i)
        {
            ptrs[i] = m_WrappedAllocator->alloc(size, alignment);
            EXPECT_NE(ptrs[i], nullptr);
            auto totalMemoryChunkDescCount = (i + 1) + initialMemoryChunkDescCount;
            EXPECT_EQ(m_WrappedAllocator->GetAllocatedBytes(), (size * (i + 1)) + sizeof(MemoryChunkDescriptor) * (totalMemoryChunkDescCount));
        }

        auto totalBytes = m_WrappedAllocator->GetAllocatedBytes();
        while (totalAllocations > 0)
        {
            totalBytes -= size;
            totalBytes -= sizeof(MemoryChunkDescriptor);
            m_WrappedAllocator->free(ptrs[(--totalAllocations) - initialMemoryChunkDescCount], alignment);
            EXPECT_EQ(m_WrappedAllocator->GetAllocatedBytes(), totalBytes);
        }

        EXPECT_EQ(m_WrappedAllocator->GetSize(), originalSize);

        delete[] ptrs;
    }

    DECLARE_EXPANDABLE_BLOCK_TEST_F(AllocateThenDeallocateInAllocationOrder)
    {
        constexpr size_t size = 8;
        constexpr size_t alignment = 8;
        constexpr uint32 initialMemoryChunkDescCount = 2;

        constexpr size_t sizeOfOneAllocation = size + sizeof(MemoryChunkDescriptor);
        size_t totalAllocations = m_WrappedAllocator->GetSize() / sizeOfOneAllocation;

        size_t originalSize = m_WrappedAllocator->GetSize();

        void** ptrs = new void*[totalAllocations];
        for (uint32 i = 0; i < totalAllocations; ++i)
        {
            ptrs[i] = m_WrappedAllocator->alloc(size, alignment);
            EXPECT_NE(ptrs[i], nullptr);
            auto totalMemoryChunkDescCount = (i + 1) + initialMemoryChunkDescCount;
            EXPECT_EQ(m_WrappedAllocator->GetAllocatedBytes(), (size * (i + 1)) + sizeof(MemoryChunkDescriptor) * (totalMemoryChunkDescCount));
        }

        auto totalBytes = m_WrappedAllocator->GetAllocatedBytes();
        for (uint32 i = 0; i < totalAllocations; ++i)
        {
            totalBytes -= size;
            totalBytes -= sizeof(MemoryChunkDescriptor);
            m_WrappedAllocator->free(ptrs[i], alignment);
            EXPECT_EQ(m_WrappedAllocator->GetAllocatedBytes(), totalBytes);
        }

        EXPECT_EQ(m_WrappedAllocator->GetSize(), originalSize);

        delete[] ptrs;
    }
} // namespace Savanna::Tests::Memory::Allocators
