/**
 * @file FreeListAllocatorTests.h
 * @author David Mohrhardt (david.mohrhardt@gmail.com)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-08
 *
 * @copyright Copyright (c) 2022
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
    // class FreeListAllocatorTest : public ::testing::Test
    // {
    // protected:
    //     FreeListAllocatorTest()
    //     {
    //         m_Root = malloc(1024);
    //         m_Size = 1024;
    //         m_FreeListAllocator = FreeListAllocator(m_Root, m_Size);
    //     }

    //     ~FreeListAllocatorTest() override
    //     {
    //         if (m_Root != nullptr)
    //         {
    //             free(m_Root);
    //         }
    //     }

    //     // If the constructor and destructor are not enough for setting up
    //     // and cleaning up each test, you can define the following methods:

    //     void SetUp() override
    //     {
    //         // For Universal Setup
    //     }

    //     void TearDown() override
    //     {
    //         // Code here will be called immediately after each test (right
    //         // before the destructor).
    //     }

    //     const uintptr_t GetRootAsUIntPtr() const
    //     {
    //         return reinterpret_cast<uintptr_t>(m_Root);
    //     }

    //     const int GetSize() const
    //     {
    //         return m_Size;
    //     }

    //     void CallAllocate(FreeListAllocator& allocator, size_t size, size_t &alignment, void** out_ptr)
    //     {
    //         *out_ptr = allocator.Allocate(size, alignment);
    //     }

    // private:
    //     void* m_Root;
    //     int m_Size;

    // protected:
    //     Savanna::FreeListAllocator m_FreeListAllocator;
    // };

    // DECLARE_FREE_LIST_TEST_F(EnsureInitializationIsCorrect)
    // {
    //     EXPECT_EQ(m_FreeListAllocator.GetSize(), GetSize());
    //     EXPECT_EQ(m_FreeListAllocator.GetAllocatedBytes(), sizeof(FreeBlockDesc));
    //     EXPECT_EQ(m_FreeListAllocator.GetSize(), GetSize());
    // }

    // DECLARE_FREE_LIST_TEST_F(EnsureAllocationDoesntThrowOnEmptyAllocator)
    // {
    //     void* result = nullptr;
    //     size_t alignment = 8;
    //     size_t size = 8;
    //     EXPECT_NO_THROW(CallAllocate(m_FreeListAllocator, size, alignment, &result));
    // }
} // namespace Savanna::Tests::Memory::Allocators
