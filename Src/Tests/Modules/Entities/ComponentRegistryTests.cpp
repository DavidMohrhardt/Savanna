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
        auto testComponentId = CustomComponentData::GetId();
        EXPECT_TRUE(se_IsValidComponentId(testComponentId));
    }

    DECLARE_COMPONENT_REGISTRY_TEST(TestTemplateSpecializationTestComponentGetsRegistered)
    {
        auto testComponentId = TemplateSpecializationTestComponent<int>::GetId();
        EXPECT_TRUE(se_IsValidComponentId(testComponentId));
    }

    DECLARE_COMPONENT_REGISTRY_TEST(TestTemplatesProduceDifferingComponentIds)
    {
        auto testComponentId1 = TemplateSpecializationTestComponent<int>::GetId();
        auto testComponentId2 = TemplateSpecializationTestComponent<float>::GetId();
        EXPECT_TRUE(se_IsValidComponentId(testComponentId1));
        EXPECT_TRUE(se_IsValidComponentId(testComponentId2));
        EXPECT_NE(testComponentId1.m_ComponentId, testComponentId2.m_ComponentId);
    }

    DECLARE_COMPONENT_REGISTRY_TEST(TestComponentIdsAreUnique)
    {
        auto testComponentId1 = CustomComponentData::GetId();
        auto testComponentId2 = TemplateSpecializationTestComponent<int>::GetId();
        EXPECT_TRUE(se_IsValidComponentId(testComponentId1));
        EXPECT_TRUE(se_IsValidComponentId(testComponentId2));
        EXPECT_NE(testComponentId1.m_ComponentId, testComponentId2.m_ComponentId);
    }

    DECLARE_COMPONENT_REGISTRY_TEST(TestArbitraryTypesProperlyGenerateIds)
    {
        SEComponentId testComponentIds[5] = {
            TemplateSpecializationTestComponent<int>::GetId(),
            TemplateSpecializationTestComponent<char>::GetId(),
            TemplateSpecializationTestComponent<float>::GetId(),
            TemplateSpecializationTestComponent<double>::GetId(),
            TemplateSpecializationTestComponent<TemplateSpecializationTestComponent<int>>::GetId()
        };
        for (int i = 0; i < 5; ++i)
        {
            EXPECT_TRUE( se_IsValidComponentId(testComponentIds[i]) );
            for (int j = 0; j < i; ++j)
            {
                if (i == j)
                {
                    continue;
                }

                EXPECT_NE(testComponentIds[i].m_ComponentId, testComponentIds[j].m_ComponentId);
            }
        }
    }

    DECLARE_COMPONENT_REGISTRY_TEST(TestIfComponentIdPastFiftySixEntriesWrapsPastZero)
    {
        int32 totalNumberOfComponentsRegistered = ComponentRegistry::GetTotalNumberOfRegisteredComponents();
        int32 expectedNumberOfComponentsInNextSet = (totalNumberOfComponentsRegistered + 64) % k_ComponentIdKeyBitCount;
        ComponentId ids[64] = {
            ArbitraryType::GetId(),
            ArbitraryType2::GetId(),
            ArbitraryType3::GetId(),
            ArbitraryType4::GetId(),
            ArbitraryType5::GetId(),
            ArbitraryType6::GetId(),
            ArbitraryType7::GetId(),
            ArbitraryType8::GetId(),
            ArbitraryType9::GetId(),
            ArbitraryType10::GetId(),
            ArbitraryType11::GetId(),
            ArbitraryType12::GetId(),
            ArbitraryType13::GetId(),
            ArbitraryType14::GetId(),
            ArbitraryType15::GetId(),
            ArbitraryType16::GetId(),
            ArbitraryType17::GetId(),
            ArbitraryType18::GetId(),
            ArbitraryType19::GetId(),
            ArbitraryType20::GetId(),
            ArbitraryType21::GetId(),
            ArbitraryType22::GetId(),
            ArbitraryType23::GetId(),
            ArbitraryType24::GetId(),
            ArbitraryType25::GetId(),
            ArbitraryType26::GetId(),
            ArbitraryType27::GetId(),
            ArbitraryType28::GetId(),
            ArbitraryType29::GetId(),
            ArbitraryType30::GetId(),
            ArbitraryType31::GetId(),
            ArbitraryType32::GetId(),
            ArbitraryType33::GetId(),
            ArbitraryType34::GetId(),
            ArbitraryType35::GetId(),
            ArbitraryType36::GetId(),
            ArbitraryType37::GetId(),
            ArbitraryType38::GetId(),
            ArbitraryType39::GetId(),
            ArbitraryType40::GetId(),
            ArbitraryType41::GetId(),
            ArbitraryType42::GetId(),
            ArbitraryType43::GetId(),
            ArbitraryType44::GetId(),
            ArbitraryType45::GetId(),
            ArbitraryType46::GetId(),
            ArbitraryType47::GetId(),
            ArbitraryType48::GetId(),
            ArbitraryType49::GetId(),
            ArbitraryType50::GetId(),
            ArbitraryType51::GetId(),
            ArbitraryType52::GetId(),
            ArbitraryType53::GetId(),
            ArbitraryType54::GetId(),
            ArbitraryType55::GetId(),
            ArbitraryType56::GetId(),
            ArbitraryType57::GetId(),
            ArbitraryType58::GetId(),
            ArbitraryType59::GetId(),
            ArbitraryType60::GetId(),
            ArbitraryType61::GetId(),
            ArbitraryType62::GetId(),
            ArbitraryType63::GetId(),
            ArbitraryType64::GetId()
        };

        bool allUnique = true;
        int alternateSetCount = 0;
        for (int i = 0; i < 64; ++i)
        {
            for (int j = 0; j < i; ++j)
            {
                if (i == j)
                {
                    continue;
                }

                auto componentId1 = ids[i];
                auto componentId2 = ids[j];
                if (componentId1.m_ComponentId == componentId2.m_ComponentId)
                {
                    allUnique = false;
                    break;
                }
            }
            if (ids[i].m_Set > 0)
                alternateSetCount++;
        }

        EXPECT_TRUE(allUnique);
        EXPECT_TRUE(alternateSetCount > 1);
        EXPECT_EQ(alternateSetCount, expectedNumberOfComponentsInNextSet);
    }
}
