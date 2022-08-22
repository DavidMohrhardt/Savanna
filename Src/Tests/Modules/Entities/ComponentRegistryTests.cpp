#include "EntitiesTestUtils.h"

#include <Entities/ComponentRegistry.h>

#include <Memory/CacheLine.h>

#define DECLARE_COMPONENT_REGISTRY_TEST(__name) \
    DECLARE_ENTITIES_TEST(ComponentRegistry, __name)

namespace Savanna::Entities::Tests
{
    using namespace Savanna;
    using namespace Savanna::Entities;

    struct CustomComponentData : IComponentData<CustomComponentData>
    {
        void* m_Data1;
        int m_Data2[5];
    };

    template<typename T>
    struct TestComponent : public IComponentData<T> {};

    template<>
    struct TestComponent<int>
    {
        int m_Value;
    };

    template<>
    struct TestComponent<float>
    {
        float m_Value;
    };

    template<>
    struct TestComponent<double>
    {
        double m_Value;
    };

    template<>
    struct TestComponent<long>
    {
        long m_Value;
    };

    template<>
    struct TestComponent<long long>
    {
        long long m_Value;
    };

    template<>
    struct TestComponent<unsigned int>
    {
        unsigned int m_Value;
    };

    template<>
    struct TestComponent<unsigned long>
    {
        unsigned long m_Value;
    };


    DECLARE_COMPONENT_REGISTRY_TEST(TestIfCustomComponentIsRegisteredAutomatically)
    {
        auto componentIdIntTemplate = ComponentRegistry::GetComponentIdFromType<CustomComponentData>();
    }

    DECLARE_COMPONENT_REGISTRY_TEST(TestIfTemplatedComponentIsRegistered)
    {
        auto componentIdIntTemplate = ComponentRegistry::GetComponentIdFromType<TestComponent<int>>();
        // ComponentRegistry registry;
        // registry.Register<TestComponent<int>>();
        // EXPECT_TRUE(registry.IsRegistered<TestComponent<int>>());
    }
}
