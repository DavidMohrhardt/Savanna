#include "BinaryTreeAllocator.h"

#include "Memory/MemoryManager.h"

namespace savanna
{
    inline size_t AlignForwardAdjust(void* ptr, const size_t alignment)
    {
        uintptr_t address = reinterpret_cast<uintptr_t>(ptr);
        size_t adjustment = alignment - (address & static_cast<uintptr_t>(alignment - 1));

        if (adjustment == alignment)
            return 0; // already aligned

        return adjustment;
    }

    BinaryTreeAllocator::BinaryTreeAllocator(size_t size, const AllocatorKind providerAllocatorKind)//void *pBuffer, size_t size)
        : m_Buffer(nullptr), m_Root(nullptr), m_AllocatorKind(providerAllocatorKind)
    {
        auto interface = MemoryManager::Get()->GetAllocatorInterfaceForAllocatorKind(m_AllocatorKind);
        m_Buffer = SAVANNA_INTERFACE_ALLOCATE(&interface, size + sizeof(FreeBlock), nullptr);

        size_t forwardAlignment = AlignForwardAdjust(m_Buffer, alignof(FreeBlock));
        m_Root = reinterpret_cast<FreeBlock*>(reinterpret_cast<uintptr_t>(m_Root) + forwardAlignment);
        *m_Root = { size - forwardAlignment - sizeof(FreeBlock), nullptr, nullptr };
    }

    BinaryTreeAllocator::~BinaryTreeAllocator()
    {
        auto interface = MemoryManager::Get()->GetAllocatorInterfaceForAllocatorKind(m_AllocatorKind);
        SAVANNA_INTERFACE_FREE(&interface, m_Buffer, nullptr);
    }

    void *BinaryTreeAllocator::Allocate(
        const size_t size,
        const size_t alignment)
    {
        // Find best fit
        FreeBlock* last = nullptr;
        FreeBlock* block = FindBestFit(&last, size, alignment);

        if (!block)
            return nullptr;

        FreeBlock* newBlock = SplitNode(block, size, alignment);
        RemoveNode(block, last);
        InsertNode(newBlock);

        return PrepareAllocatedBlock(block, size, alignment);
    }

    void BinaryTreeAllocator::Deallocate(void *ptr)
    {
        AllocatedBlock* block = reinterpret_cast<AllocatedBlock*>(reinterpret_cast<uintptr_t>(ptr) - sizeof(AllocatedBlock));
        FreeBlock* adjacentBlock = MergeWithAdjacentAddressIfPossible(block);
        if (adjacentBlock)
        {
            RemoveNode(adjacentBlock, nullptr);
            InsertNode(adjacentBlock);
        }
        else
        {
            FreeBlock* newBlock = reinterpret_cast<FreeBlock*>(block);
            *newBlock = { block->m_Size, nullptr, nullptr };
            InsertNode(newBlock);
        }
    }

    void BinaryTreeAllocator::InsertNode(FreeBlock *node)
    {
        if (!m_Root)
        {
            m_Root = node;
            return;
        }

        FreeBlock* current = m_Root;
        while (true)
        {
            if (node < current)
            {
                if (current->m_Left)
                    current = current->m_Left;
                else
                {
                    current->m_Left = node;
                    break;
                }
            }
            else
            {
                if (current->m_Right)
                    current = current->m_Right;
                else
                {
                    current->m_Right = node;
                    break;
                }
            }
        }
    }

    FreeBlock *BinaryTreeAllocator::MergeWithAdjacentAddressIfPossible(AllocatedBlock *block)
    {
        // This one is a bit tricky. We need to find the block that is adjacent to the one we are freeing.
        // We can do this by traversing the tree and checking if the address of the block we are freeing
        // is adjacent to the address of the current node. If it is, we can merge the two blocks together.

        FreeBlock* current = m_Root;
        while (current)
        {
            if (reinterpret_cast<uintptr_t>(block) < reinterpret_cast<uintptr_t>(current))
            {
                if (reinterpret_cast<uintptr_t>(block) + block->m_Size == reinterpret_cast<uintptr_t>(current))
                {
                    block->m_Size += current->m_Size + sizeof(FreeBlock);
                    return current;
                }
                else
                    current = current->m_Left;
            }
            else
            {
                if (reinterpret_cast<uintptr_t>(current) + current->m_Size + sizeof(FreeBlock) == reinterpret_cast<uintptr_t>(block))
                {
                    current->m_Size += block->m_Size + sizeof(FreeBlock);
                    return current;
                }
                else
                    current = current->m_Right;
            }
        }
        return nullptr;
    }

    FreeBlock *BinaryTreeAllocator::FindBestFit(
        FreeBlock **outParent,
        const size_t size,
        const size_t alignment)
    {
        FreeBlock* current = m_Root;
        FreeBlock* bestFit = nullptr;
        size_t bestFitSize = 0;
        while (current)
        {
            size_t forwardAlignment = AlignForwardAdjust(current, alignment);
            size_t totalSize = size + forwardAlignment;
            if (current->m_Size >= totalSize)
            {
                if (!bestFit || current->m_Size < bestFitSize)
                {
                    bestFit = current;
                    bestFitSize = current->m_Size;
                }
            }

            if (reinterpret_cast<uintptr_t>(current) > reinterpret_cast<uintptr_t>(bestFit))
                current = current->m_Left;
            else
                current = current->m_Right;
        }

        return bestFit;
    }

    void BinaryTreeAllocator::RemoveNode(FreeBlock *node, FreeBlock* parent)
    {
        if (!node->m_Left && !node->m_Right)
        {
            if (parent)
            {
                if (parent->m_Left == node)
                    parent->m_Left = nullptr;
                else
                    parent->m_Right = nullptr;
            }
            else
                m_Root = nullptr;
        }
        else if (!node->m_Right)
        {
            if (parent)
            {
                if (parent->m_Left == node)
                    parent->m_Left = node->m_Left;
                else
                    parent->m_Right = node->m_Left;
            }
            else
                m_Root = node->m_Left;
        }
        else if (!node->m_Left)
        {
            if (parent)
            {
                if (parent->m_Left == node)
                    parent->m_Left = node->m_Right;
                else
                    parent->m_Right = node->m_Right;
            }
            else
                m_Root = node->m_Right;
        }
        else
        {
            FreeBlock* leftMost = node->m_Right;
            FreeBlock* leftMostParent = node;
            while (leftMost->m_Left)
            {
                leftMostParent = leftMost;
                leftMost = leftMost->m_Left;
            }

            leftMostParent->m_Left = leftMost->m_Right;
            leftMost->m_Left = node->m_Left;
            leftMost->m_Right = node->m_Right;

            if (parent)
            {
                if (parent->m_Left == node)
                    parent->m_Left = leftMost;
                else
                    parent->m_Right = leftMost;
            }
            else
                m_Root = leftMost;
        }
    }

    FreeBlock *BinaryTreeAllocator::SplitNode(
        FreeBlock *node,
        const size_t size,
        const size_t alignment)
    {
        size_t forwardAlignment = AlignForwardAdjust(node, alignment);
        size_t totalSize = size + forwardAlignment;
        // If this block is too small to split into two, return it
        if (node->m_Size <= totalSize + sizeof(FreeBlock))
            return nullptr;

        FreeBlock* newBlock = reinterpret_cast<FreeBlock*>(reinterpret_cast<uintptr_t>(node) + totalSize);
        *newBlock = { node->m_Size - totalSize, nullptr, nullptr };
        return newBlock;
    }

    void* BinaryTreeAllocator::PrepareAllocatedBlock(
        FreeBlock *node,
        const size_t size,
        const size_t alignment)
    {
        AllocatedBlock* allocatedBlock = reinterpret_cast<AllocatedBlock*>(node);
        size_t forwardAlignment = AlignForwardAdjust(node, alignment);
        size_t totalSize = size + forwardAlignment;
        allocatedBlock->m_Size = size;
        allocatedBlock->m_Padding = forwardAlignment;
        return reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(allocatedBlock) + sizeof(AllocatedBlock) + forwardAlignment);
    }
} // namespace Savanna
