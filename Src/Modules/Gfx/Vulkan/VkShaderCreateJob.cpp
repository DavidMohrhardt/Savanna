#include "VkShaderCreateJob.h"

namespace Savanna::Gfx::Vk
{
    void VkShaderModuleCreationJob::Execute(void *pData)
    {
        VkResult result = vkCreateShaderModule(m_ShaderModuleCreateInfo.device, &m_ShaderModuleCreateInfo, nullptr, &m_OutShaderModule);
        return (result == VK_SUCCESS);
    }
}
