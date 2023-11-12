/**
 * @file VkBuffer.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-11-10
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "SavannaVk2.h"
#include "VkGpu.h"

namespace Savanna::Gfx::Vk2
{

    class VkGpuBuffer
    {
    private:
        struct VkCpuBuffer
        {
            void* m_CpuBuffer;
            size_t m_Size;
            bool m_NeedsUpdate;
            AllocatorKind m_AllocatorKind;
        };

        VkBuffer m_GpuBuffer;
        VkCpuBuffer m_CpuBuffer;

    public:
        VkGpuBuffer(AllocatorKind allocatorKind = kSavannaAllocatorKindGeneral);

        VkGpuBuffer(void* pData, size_t sizeInBytes, AllocatorKind allocatorKind = kSavannaAllocatorKindNone);

        ~VkGpuBuffer();

        void Update(void* pData, size_t sizeInBytes);
        void FlushToGpu();
    };
} // namespace Savanna::Gfx::Vk
