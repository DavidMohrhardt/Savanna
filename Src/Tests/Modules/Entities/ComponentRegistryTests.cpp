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
        auto nextComponentId = ComponentRegistry::GetNextAvailableComponentId();

        EXPECT_GT(nextComponentId.m_SetMask, 1);
    }
}
