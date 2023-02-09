/**
 * @file MemoryView.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-02-06
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "SavannaEngine.h"
#include "Utilities/SavannaCoding.h"

namespace Savanna
{
    class MemoryView
    {
    private:
        void* m_Root = nullptr;
        size_t m_Size = 0;
    public:
        MemoryView() = default;
        MemoryView(void* root, size_t size);
        ~MemoryView() = default;
    public:
        SAVANNA_NO_DISCARD void* GetRoot() const;
        SAVANNA_NO_DISCARD size_t GetSize() const;

        // Array index operator
        SAVANNA_NO_DISCARD const byte operator[](size_t index) const;
    };
} // namespace Savanna
