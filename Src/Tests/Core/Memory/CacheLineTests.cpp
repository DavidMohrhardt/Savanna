#include "MemoryTestUtils.h"

#include <Types/Memory/CacheLine.h>

#define DECLARE_CACHE_LINE_TEST(__name) \
    DECLARE_MEMORY_TEST(CacheLineTest, __name)

namespace Savanna::Core::Tests
{
    using namespace Savanna;

    struct TestStruct
    {
        int a;
        int b;
        int c;
        int d;
        int e;
        int f;
        int g;
        int h;
        int i;
        int j;
        int k;
        int l;
        int m;
        int n;
        int o;
        int p;
        int q;
        int r;
        int s;
        int t;
        int u;
        int v;
        int w;
        int x;
        int y;
        int z;
    };

    union CacheWidthTestUnion
    {
        TestStruct m_TestStruct;
        L1CacheLine m_CacheLines[];
    };

    // TODO @DavidMohrhardt Parameterize this test https://google.github.io/googletest/advanced.html#value-parameterized-tests
    DECLARE_CACHE_LINE_TEST(GetL1CacheLineCountReturnsCorrectValue)
    {
        EXPECT_EQ(1, GetL1CacheLineCount( L1CacheLineLength() ));
        EXPECT_EQ(2, GetL1CacheLineCount( L1CacheLineLength() * 2 ));
        EXPECT_EQ(3, GetL1CacheLineCount( L1CacheLineLength() * 3 ));
        EXPECT_EQ(4, GetL1CacheLineCount( L1CacheLineLength() * 4 ));
        EXPECT_EQ(5, GetL1CacheLineCount( L1CacheLineLength() * 5 ));
    }

    DECLARE_CACHE_LINE_TEST(GetL1CacheLineCountReturnsCorrectValueForNonCachelineMultiples)
    {
        EXPECT_EQ(1, GetL1CacheLineCount( L1CacheLineLength() - 1 ));
        EXPECT_EQ(2, GetL1CacheLineCount( L1CacheLineLength() + 1 ));
        EXPECT_EQ(2, GetL1CacheLineCount( L1CacheLineLength() * 2 - 1 ));
        EXPECT_EQ(3, GetL1CacheLineCount( L1CacheLineLength() * 2 + 1 ));
        EXPECT_EQ(3, GetL1CacheLineCount( L1CacheLineLength() * 3 - 1 ));
        EXPECT_EQ(4, GetL1CacheLineCount( L1CacheLineLength() * 3 + 1 ));
        EXPECT_EQ(4, GetL1CacheLineCount( L1CacheLineLength() * 4 - 1 ));
        EXPECT_EQ(5, GetL1CacheLineCount( L1CacheLineLength() * 4 + 1 ));
    }

    DECLARE_CACHE_LINE_TEST(GetL1CacheLineCountReturnsCorrectValueForLargeStruct)
    {
        EXPECT_EQ(GetL1CacheLineCount( sizeof(L1CacheWidthContainer<TestStruct>) ), GetL1CacheLineCount( sizeof(TestStruct) ));
    }

    DECLARE_CACHE_LINE_TEST(L1CacheWidthContainerFitsUnionWithCacheLineArrayMember)
    {
        EXPECT_EQ(sizeof(CacheWidthTestUnion), sizeof(L1CacheWidthContainer<TestStruct>));
    }

} // namespace Savanna::Tests::Core::Memory
