#include "VkShaderCreateJob.h"

namespace Savanna::Gfx::Vk
{
    bool VkShaderModuleCreationJob::Execute()
    {
        VkResult result = vkCreateShaderModule(m_Device, &m_ShaderModuleCreateInfo, nullptr, &m_OutShaderModule);

        if (result != VK_SUCCESS)
        {
            // SAVANNA_ERROR("Failed to create shader module!");
        }
    }
}
