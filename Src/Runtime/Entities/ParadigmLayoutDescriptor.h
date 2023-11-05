/**
 * @file ParadigmLayoutDescriptor.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-09-09
 *
 */
#pragma once

#include "Public/ISavannaEntities.hpp"

namespace Savanna::Entities
{
    /**
     * @brief Describes the information needed to calculate the layout of a component paradigm.
     *
     * @note The pointers in this struct are not owned by this struct and
     *     must be valid for the lifetime of this struct. They exist in the
     *     paradigm's memory pool at the head of the pool.
     */
    struct ParadigmLayoutDescriptor
    {
        const se_ComponentKey_t m_ParadigmComponentKey;
        size_t m_TotalSize;
        size_t m_NumberOfComponents;
        const size_t* m_pComponentSizes;
        const size_t* m_pComponentAlignments;
    };
}
