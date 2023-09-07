/**
 * @file Allocator.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2022-08-19
 *
 */
#pragma once

#include <Utilities/SavannaCoding.h>

#include "AllocatorUtils.h"
#include "IAllocator.h"

namespace Savanna
{
    class Allocator : public IAllocator, NonCopyable
    {
    protected:
        void* m_Root;

    public:
        Allocator();
        Allocator(void* root);
        Allocator(Allocator&& other);

        virtual ~Allocator() {}
    public:
        Allocator& operator=(Allocator&& other);

        bool operator==(const Allocator& other) const { return m_Root == other.m_Root; }

    public:
        const void* GetRoot() const { return m_Root; };

        SAVANNA_NO_DISCARD bool IsValid() const { return m_Root != nullptr && GetSize() > 0; }
    };
} // namespace Savanna
