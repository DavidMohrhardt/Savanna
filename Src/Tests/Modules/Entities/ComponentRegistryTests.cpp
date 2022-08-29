#include "EntitiesTestUtils.h"
#include "ComponentRegistryTests.h"

#include <Entities/ComponentRegistry.h>

#include <Memory/CacheLine.h>

#define DECLARE_COMPONENT_REGISTRY_TEST(__name) \
    DECLARE_ENTITIES_TEST(ComponentRegistry, __name)

namespace Savanna::Entities::Tests
{
    // using namespace Savanna;
    using namespace Savanna::Entities;

    DECLARE_COMPONENT_REGISTRY_TEST(TestCustomComponentDataGetsRegistered)
    {
        auto testComponentKey = CustomComponentData::GetKey();
        EXPECT_TRUE(SEIsValidComponentKey(testComponentKey));
    }

    DECLARE_COMPONENT_REGISTRY_TEST(TestTemplateSpecializationTestComponentGetsRegistered)
    {
        auto testComponentKey = TemplateSpecializationTestComponent<int>::GetKey();
        EXPECT_TRUE(SEIsValidComponentKey(testComponentKey));
    }

    DECLARE_COMPONENT_REGISTRY_TEST(TestTemplatesProduceDifferingComponentKeys)
    {
        auto testComponentKey1 = TemplateSpecializationTestComponent<int>::GetKey();
        auto testComponentKey2 = TemplateSpecializationTestComponent<float>::GetKey();
        EXPECT_TRUE(SEIsValidComponentKey(testComponentKey1));
        EXPECT_TRUE(SEIsValidComponentKey(testComponentKey2));
        EXPECT_NE(testComponentKey1.m_FullComponentKey, testComponentKey2.m_FullComponentKey);
    }

    DECLARE_COMPONENT_REGISTRY_TEST(TestComponentKeysAreUnique)
    {
        auto testComponentKey1 = CustomComponentData::GetKey();
        auto testComponentKey2 = TemplateSpecializationTestComponent<int>::GetKey();
        EXPECT_TRUE(SEIsValidComponentKey(testComponentKey1));
        EXPECT_TRUE(SEIsValidComponentKey(testComponentKey2));
        EXPECT_NE(testComponentKey1.m_FullComponentKey, testComponentKey2.m_FullComponentKey);
    }

    DECLARE_COMPONENT_REGISTRY_TEST(TestArbitraryTypesProperlyGenerateIds)
    {
        SEComponentKey testComponentKeys[5] = {
            TemplateSpecializationTestComponent<int>::GetKey(),
            TemplateSpecializationTestComponent<char>::GetKey(),
            TemplateSpecializationTestComponent<float>::GetKey(),
            TemplateSpecializationTestComponent<double>::GetKey(),
            TemplateSpecializationTestComponent<TemplateSpecializationTestComponent<int>>::GetKey()
        };
        for (int i = 0; i < 5; ++i)
        {
            EXPECT_TRUE( SEIsValidComponentKey(testComponentKeys[i]) );
            for (int j = 0; j < i; ++j)
            {
                if (i == j)
                {
                    continue;
                }

                EXPECT_NE(testComponentKeys[i].m_FullComponentKey, testComponentKeys[j].m_FullComponentKey);
            }
        }
    }

    DECLARE_COMPONENT_REGISTRY_TEST(TestIfComponentKeyPast26EntriesWrapsPastZero)
    {
        constexpr int k_NumberOfTestElements = SAVANNA_ECS_KEY_SET_BIT_COUNT + SAVANNA_ECS_KEY_BIT_COUNT;
        int32 expectedNumberOfComponentsInNextSet = (ComponentRegistry::GetTotalNumberOfRegisteredComponents() + k_NumberOfTestElements) % SAVANNA_ECS_KEY_BIT_COUNT;
        ComponentKey ids[k_NumberOfTestElements] = {
            ArbitraryType::GetKey(),
            ArbitraryType2::GetKey(),
            ArbitraryType3::GetKey(),
            ArbitraryType4::GetKey(),
            ArbitraryType5::GetKey(),
            ArbitraryType6::GetKey(),
            ArbitraryType7::GetKey(),
            ArbitraryType8::GetKey(),
            ArbitraryType9::GetKey(),
            ArbitraryType10::GetKey(),
            ArbitraryType11::GetKey(),
            ArbitraryType12::GetKey(),
            ArbitraryType13::GetKey(),
            ArbitraryType14::GetKey(),
            ArbitraryType15::GetKey(),
            ArbitraryType16::GetKey(),
            ArbitraryType17::GetKey(),
            ArbitraryType18::GetKey(),
            ArbitraryType19::GetKey(),
            ArbitraryType20::GetKey(),
            ArbitraryType21::GetKey(),
            ArbitraryType22::GetKey(),
            ArbitraryType23::GetKey(),
            ArbitraryType24::GetKey(),
            ArbitraryType25::GetKey(),
            ArbitraryType26::GetKey(),
            ArbitraryType27::GetKey(),
            ArbitraryType28::GetKey(),
            ArbitraryType29::GetKey(),
            ArbitraryType30::GetKey(),
            ArbitraryType31::GetKey(),
            ArbitraryType32::GetKey(),
        };

        bool allUnique = true;
        int alternateSetCount = 0;
        for (int i = 0; i < SAVANNA_ECS_KEY_BIT_COUNT; ++i)
        {
            if (ids[i].m_Set != 0)
            {
                ++alternateSetCount;
            }

            for (int j = 0; j < i; ++j)
            {
                if (i == j)
                {
                    continue;
                }

                auto componentId1 = ids[i];
                auto componentId2 = ids[j];
                if (componentId1.m_FullComponentKey == componentId2.m_FullComponentKey)
                {
                    allUnique = false;
                    break;
                }
            }
        }

        EXPECT_TRUE(allUnique);
        EXPECT_TRUE(alternateSetCount > 1);
        EXPECT_EQ(alternateSetCount, expectedNumberOfComponentsInNextSet);
    }
}
