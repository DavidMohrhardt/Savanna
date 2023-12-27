#include "VkBuffer.h"

namespace savanna::gfx::vk
{
    void VkGpuBuffer::Update(void* pData, size_t sizeInBytes)
    {
        // SAVANNA_FREE()
        // m_CpuBuffer.Resize(sizeInBytes);
        // ::memcpy(m_CpuBuffer.GetBuffer(), pData, sizeInBytes);
        // m_NeedsUpdate = true;
    }

    void VkGpuBuffer::FlushToGpu()
    {
        // if (m_NeedsUpdate && m_CpuBuffer.IsValid())
        // {
        //     // VkBufferCreateInfo
        // }
    }
} // namespace savanna::Gfx::vk
