#pragma once

#define DECLARE_COMPONENT_REGISTRY_TEST(__name) \
    DECLARE_ENTITIES_TEST(ComponentRegistry, __name)

namespace Savanna::Entities::Tests
{
    // using namespace Savanna;
    using namespace Savanna::Entities;

    // Generate 64 arbitrary types to test the component registry
    struct ArbitraryType : public IComponentData<ArbitraryType> {};
    struct ArbitraryType2 : public IComponentData<ArbitraryType2> {};
    struct ArbitraryType3 : public IComponentData<ArbitraryType3> {};
    struct ArbitraryType4 : public IComponentData<ArbitraryType4> {};
    struct ArbitraryType5 : public IComponentData<ArbitraryType5> {};
    struct ArbitraryType6 : public IComponentData<ArbitraryType6> {};
    struct ArbitraryType7 : public IComponentData<ArbitraryType7> {};
    struct ArbitraryType8 : public IComponentData<ArbitraryType8> {};
    struct ArbitraryType9 : public IComponentData<ArbitraryType9> {};
    struct ArbitraryType10 : public IComponentData<ArbitraryType10> {};
    struct ArbitraryType11 : public IComponentData<ArbitraryType11> {};
    struct ArbitraryType12 : public IComponentData<ArbitraryType12> {};
    struct ArbitraryType13 : public IComponentData<ArbitraryType13> {};
    struct ArbitraryType14 : public IComponentData<ArbitraryType14> {};
    struct ArbitraryType15 : public IComponentData<ArbitraryType15> {};
    struct ArbitraryType16 : public IComponentData<ArbitraryType16> {};
    struct ArbitraryType17 : public IComponentData<ArbitraryType17> {};
    struct ArbitraryType18 : public IComponentData<ArbitraryType18> {};
    struct ArbitraryType19 : public IComponentData<ArbitraryType19> {};
    struct ArbitraryType20 : public IComponentData<ArbitraryType20> {};
    struct ArbitraryType21 : public IComponentData<ArbitraryType21> {};
    struct ArbitraryType22 : public IComponentData<ArbitraryType22> {};
    struct ArbitraryType23 : public IComponentData<ArbitraryType23> {};
    struct ArbitraryType24 : public IComponentData<ArbitraryType24> {};
    struct ArbitraryType25 : public IComponentData<ArbitraryType25> {};
    struct ArbitraryType26 : public IComponentData<ArbitraryType26> {};
    struct ArbitraryType27 : public IComponentData<ArbitraryType27> {};
    struct ArbitraryType28 : public IComponentData<ArbitraryType28> {};
    struct ArbitraryType29 : public IComponentData<ArbitraryType29> {};
    struct ArbitraryType30 : public IComponentData<ArbitraryType30> {};
    struct ArbitraryType31 : public IComponentData<ArbitraryType31> {};
    struct ArbitraryType32 : public IComponentData<ArbitraryType32> {};
    struct ArbitraryType33 : public IComponentData<ArbitraryType33> {};
    struct ArbitraryType34 : public IComponentData<ArbitraryType34> {};
    struct ArbitraryType35 : public IComponentData<ArbitraryType35> {};
    struct ArbitraryType36 : public IComponentData<ArbitraryType36> {};
    struct ArbitraryType37 : public IComponentData<ArbitraryType37> {};
    struct ArbitraryType38 : public IComponentData<ArbitraryType38> {};
    struct ArbitraryType39 : public IComponentData<ArbitraryType39> {};
    struct ArbitraryType40 : public IComponentData<ArbitraryType40> {};
    struct ArbitraryType41 : public IComponentData<ArbitraryType41> {};
    struct ArbitraryType42 : public IComponentData<ArbitraryType42> {};
    struct ArbitraryType43 : public IComponentData<ArbitraryType43> {};
    struct ArbitraryType44 : public IComponentData<ArbitraryType44> {};
    struct ArbitraryType45 : public IComponentData<ArbitraryType45> {};
    struct ArbitraryType46 : public IComponentData<ArbitraryType46> {};
    struct ArbitraryType47 : public IComponentData<ArbitraryType47> {};
    struct ArbitraryType48 : public IComponentData<ArbitraryType48> {};
    struct ArbitraryType49 : public IComponentData<ArbitraryType49> {};
    struct ArbitraryType50 : public IComponentData<ArbitraryType50> {};
    struct ArbitraryType51 : public IComponentData<ArbitraryType51> {};
    struct ArbitraryType52 : public IComponentData<ArbitraryType52> {};
    struct ArbitraryType53 : public IComponentData<ArbitraryType53> {};
    struct ArbitraryType54 : public IComponentData<ArbitraryType54> {};
    struct ArbitraryType55 : public IComponentData<ArbitraryType55> {};
    struct ArbitraryType56 : public IComponentData<ArbitraryType56> {};
    struct ArbitraryType57 : public IComponentData<ArbitraryType57> {};
    struct ArbitraryType58 : public IComponentData<ArbitraryType58> {};
    struct ArbitraryType59 : public IComponentData<ArbitraryType59> {};
    struct ArbitraryType60 : public IComponentData<ArbitraryType60> {};
    struct ArbitraryType61 : public IComponentData<ArbitraryType61> {};
    struct ArbitraryType62 : public IComponentData<ArbitraryType62> {};
    struct ArbitraryType63 : public IComponentData<ArbitraryType63> {};
    struct ArbitraryType64 : public IComponentData<ArbitraryType64> {};

    // Just a custom named component
    struct CustomComponentData : public IComponentData<CustomComponentData>
    {
        void* m_Data1;
        int m_Data2[5];
    };

    // A templated component
    template<typename T>
    struct TemplateSpecializationTestComponent : public IComponentData<T>
    {
        T m_Data;
    };
}
