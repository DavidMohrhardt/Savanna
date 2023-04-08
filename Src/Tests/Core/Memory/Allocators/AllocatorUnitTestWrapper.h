/**
 * @file AllocatorUnitTestWrapper.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2023-04-07
 *
 */
#include "AllocatorTestUtils.h"
#include <Types/Memory/Allocator.h>

#include <memory>

namespace Savanna::Core::Tests
{
    template <typename TAllocator>
    requires std::is_base_of_v<Allocator, TAllocator>
    class AllocatorUnitTestWrapper
    {
    private:
        using AllocatorType = TAllocator;
        AllocatorType m_Allocator;

        AllocatorUnitTestWrapper() = delete;

    public:
        template <typename... Args>
        AllocatorUnitTestWrapper(Args&&... args)
            : m_Allocator(std::forward<Args>(args)...)
        {}

        ~AllocatorUnitTestWrapper() {}

    private:
        // TODO @DavidMohrhardt: Refactor the test framework for allocators to use the new allocator interface
        inline bool ValidateAllocation(
            void* const ptr,
            const size_t& oldAllocatedBytes,
            const size_t& minimalSizeIncrease,
            const size_t& alignment,
            const bool& expectFailure)
        {
            if (expectFailure)
            {
                EXPECT_EQ(ptr, nullptr);
                EXPECT_EQ(m_Allocator.GetAllocatedBytes(), oldAllocatedBytes);
            }
            else
            {
                EXPECT_NE(ptr, nullptr);
                // Can only check if the allocated bytes increased within the minimal size increase
                EXPECT_GE(m_Allocator.GetAllocatedBytes(), oldAllocatedBytes + minimalSizeIncrease);
                // Check for alignment
                EXPECT_EQ(reinterpret_cast<const uintptr>(ptr) % alignment, 0);
            }

            return true;
        }

        inline bool ValidateDeallocation(
            void* const ptr,
            const size_t& oldAllocatedBytes,
            const size_t& minimalSizeDecrease,
            const size_t& alignment,
            const bool& expectFailure)
        {
            if (expectFailure)
            {
                EXPECT_EQ(ptr, nullptr);
                EXPECT_EQ(m_Allocator.GetAllocatedBytes(), oldAllocatedBytes);
            }
            else
            {
                EXPECT_NE(ptr, nullptr);
                EXPECT_LE(m_Allocator.GetAllocatedBytes(), oldAllocatedBytes - minimalSizeDecrease);
                EXPECT_TRUE(AllocatorUtils::IsAligned(ptr, alignment));
            }

            return true;
        }

        inline void* alloc_UnitTest(
            const size_t& size,
            const size_t& alignment,
            bool expectFailure)
        {
            size_t oldAllocatedBytes = m_Allocator.GetAllocatedBytes();
            const void* oldHead = m_Allocator.GetRoot();

            void* ptr = nullptr;
            EXPECT_NO_THROW(ptr = m_Allocator.alloc(size, alignment));

            ValidateAllocation(ptr, oldAllocatedBytes, size, alignment, expectFailure);

            return ptr;
        }

        inline void free_UnitTest(void* const ptr, const size_t& alignment, bool expectFailure)
        {
            auto oldAllocatedBytes = m_Allocator.GetAllocatedBytes();
            EXPECT_NO_THROW(m_Allocator.free(ptr, alignment));

            // No type information available, so we can only check if the allocated bytes decreased here
            EXPECT_LE(m_Allocator.GetAllocatedBytes(), oldAllocatedBytes);
        }

        template<typename T>
        inline bool Allocate_UnitTest(const size_t& count, bool expectFailure)
        {
            size_t oldAllocatedBytes = m_Allocator.GetAllocatedBytes();
            size_t minimalSizeIncrease = sizeof(T) * count;
            T* ptr = !expectFailure
                ? EXPECT_NO_THROW(m_Allocator.Allocate<T>(count))
                : EXPECT_THROW(m_Allocator.Allocate<T>(count), std::bad_alloc);

            ValidateAllocation(ptr, oldAllocatedBytes, minimalSizeIncrease, alignof(T), expectFailure);

            return ptr;
        }

        template<typename T>
        inline void Release_UnitTest(void* const ptr, bool expectFailure)
        {
            size_t oldAllocatedBytes = m_Allocator.GetAllocatedBytes();
            size_t minimalSizeIncrease = sizeof(T);

            if (!expectFailure)
            {
                EXPECT_NO_THROW(m_Allocator.Release<T>(ptr));
            }
            else
            {
                EXPECT_THROW(m_Allocator.Release<T>(ptr), std::bad_alloc);
            }

            ValidateDeallocation(ptr, oldAllocatedBytes, minimalSizeIncrease, alignof(T), expectFailure);
        }

    public:
        inline AllocatorType& GetAllocator() const { return m_Allocator; }
        inline size_t GetAllocatedBytes() const { return m_Allocator.GetAllocatedBytes(); }
        inline const void* GetRoot() const { return m_Allocator.GetRoot(); }
        inline size_t GetSize() const { return m_Allocator.GetSize(); }

        void* alloc(const size_t& size, const size_t& alignment, bool expectFailure = false)
        {
            return alloc_UnitTest(size, alignment, expectFailure);
        }

        void free(void* const ptr, const size_t& alignment, bool expectFailure = false)
        {
            free_UnitTest(ptr, alignment, expectFailure);
        }

        template<typename T>
        T* Allocate(const size_t& count = 1, bool expectFailure = false)
        {
            return Allocate_UnitTest<T>(count, expectFailure);
        }

        template<typename T>
        void Release(T* const ptr, bool expectFailure = false)
        {
            Release_UnitTest<T>(ptr, expectFailure);
        }
    };
} // namespace Savanna::Testing
