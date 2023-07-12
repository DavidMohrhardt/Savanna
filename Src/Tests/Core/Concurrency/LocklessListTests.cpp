#if ENABLE_LOCKLESS_LIST_TESTS
/**
 * @file LocklessListTests.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-05-27
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "ConcurrencyTestUtils.h"

#include "Core/Types/Containers/Concurrent/LocklessList.h"

#define DECLARE_LOCKLESS_LIST_TEST_F(__name) \
    TEST_F(LocklessListTests, __name)

namespace Savanna::Core::Tests
{
    class LocklessListTests : public ConcurrencyTestFramework
    {
    protected:
        LocklessList<int> m_List;

    public:
        LocklessListTests()
            : ConcurrencyTestFramework(std::thread::hardware_concurrency())
        {
        }

        virtual ~LocklessListTests() = default;

        void SetUp() override
        {
            // Do nothing
        }

        void TearDown() override
        {
            JoinThreads();
        }
    };

    DECLARE_LOCKLESS_LIST_TEST_F(ConcurrentAddTest)
    {
        m_List.Add(0);
        RunThreads([=]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            auto curNodeCount = m_List.Count();
            m_List.Add(curNodeCount);
        });
        JoinThreads();
        ASSERT_EQ(m_List.Count(), GetThreadCount() + 1);

    }
}
#endif
