/**
 * @file MemoryArenaTests.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-08
 *
 */

#include "MemoryTestUtils.h"

#include <Memory/MemoryArena.h>

#define DECLARE_MEMORY_ARENA_TEST(__subFixture, __name) \
    DECLARE_MEMORY_TEST(MemoryArena##__subFixture, __name)

#define DECLARE_MEMORY_ARENA_TEST_F(__name) \
    TEST_F(MemoryArenaTest, __name)

namespace Savanna::Core::Tests
{
#if ENABLE_MEMORY_ARENA_TESTS
    class MemoryArenaTest : public ::testing::Test
    {
    public:
        const size_t k_DefaultArenaSize = 1024u * 2;
    protected:
        MemoryArenaTest()
            : m_Arena(k_DefaultArenaSize)
        {
        }

        ~MemoryArenaTest() override
        {
        }

    protected:
        void* AllocateMemory(const size_t& size)
        {
            return m_Arena.AcquireMemory(size);
        }

        MemoryArena m_Arena;
    };

    DECLARE_MEMORY_ARENA_TEST_F(EnsureInitializationIsCorrect)
    {
        EXPECT_EQ(m_Arena.GetSize(), k_DefaultArenaSize);
        EXPECT_EQ(m_Arena.GetAllocated(), 0);
        EXPECT_EQ(m_Arena.GetMaxSize(), k_DefaultArenaSize);
    }

    DECLARE_MEMORY_ARENA_TEST_F(EnsureLargeValidAllocationSucceeds)
    {
        void* ptr = m_Arena.AcquireMemory(k_DefaultArenaSize);
        EXPECT_NE(ptr, nullptr);
        EXPECT_EQ(m_Arena.GetSize(), k_DefaultArenaSize);
        EXPECT_EQ(m_Arena.GetAllocated(), k_DefaultArenaSize);
        EXPECT_EQ(m_Arena.GetMaxSize(), 0);
    }

    DECLARE_MEMORY_ARENA_TEST_F(EnsureSequentialAllocationsSucceed)
    {
        const size_t allocationsSize = k_DefaultArenaSize / L1CacheLineLength();
        for (int i = 0; i < m_Arena.GetMaxSize(); i+=allocationsSize)
        {
            void* ptr = m_Arena.AcquireMemory(allocationsSize);
            EXPECT_NE(ptr, nullptr);
            EXPECT_EQ(m_Arena.GetSize(), k_DefaultArenaSize);
            EXPECT_EQ(m_Arena.GetAllocated(), i + allocationsSize);
            EXPECT_EQ(m_Arena.GetMaxSize(), k_DefaultArenaSize - i - allocationsSize);
        }
    }

    DECLARE_MEMORY_ARENA_TEST_F(EnsureInvalidAllocationThrows)
    {
        void* ptr = m_Arena.AcquireMemory(k_DefaultArenaSize + 1);
        EXPECT_EQ(ptr, nullptr);
        ptr = m_Arena.AcquireMemory(k_DefaultArenaSize);
        EXPECT_NE(ptr, nullptr);
        EXPECT_EQ(m_Arena.GetSize(), k_DefaultArenaSize);
        EXPECT_EQ(m_Arena.GetAllocated(), k_DefaultArenaSize);
        EXPECT_EQ(m_Arena.GetMaxSize(), 0);
        ptr = m_Arena.AcquireMemory(1024);
        EXPECT_EQ(ptr, nullptr);
    }
#endif // end ENABLE_MEMORY_ARENA_TESTS
} // namespace Savanna::Tests
