/**
 * @file FreeListAllocator.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-10-18
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "Memory/MemoryLabel.h"

#include <cstdint>

namespace Savanna
{

    struct FreeBlock
    {
        size_t m_Size;
        FreeBlock* m_Left;
        FreeBlock* m_Right;
    };

    struct AllocatedBlock
    {
        size_t m_Size;
        size_t m_ForwardAdjustment;
        intptr_t m_Padding = 0xDEADBEEFDEADBEEF;
    };

    class BinaryTreeAllocator
    {
    private:
        static_assert(sizeof(FreeBlock) == sizeof(AllocatedBlock), "FreeBlock and AllocatedBlock must be the same size");
        MemoryLabel m_Label;

        void* m_Buffer;
        // The tracking is implemented as a balanced binary tree
        FreeBlock* m_Root;

    public:

        BinaryTreeAllocator(size_t size, const MemoryLabel providerLabel = k_SavannaMemoryLabelHeap);
        ~BinaryTreeAllocator();

        void* Allocate(const size_t size, const size_t alignment);
        void Deallocate(void* ptr);

    private:
        BinaryTreeAllocator(const BinaryTreeAllocator& other);
        BinaryTreeAllocator& operator=(const BinaryTreeAllocator& other);

        void InsertNode(FreeBlock* node);
        void RemoveNode(FreeBlock* node, FreeBlock* parent);

        FreeBlock* MergeWithAdjacentAddressIfPossible(AllocatedBlock* block);

        FreeBlock* FindBestFit(FreeBlock** outParent, const size_t size, const size_t alignment);

        FreeBlock* SplitNode(FreeBlock* node, const size_t size, const size_t alignment);

        void* PrepareAllocatedBlock(FreeBlock* node, const size_t size, const size_t alignment);
    };

} // namespace Savanna
