#include "VkDriver.h"

#include "SavannaVk2.h"

#include "Utilities/VkInfoCreateUtils.h"

namespace Savanna::Gfx::Vk2
{
    se_GfxErrorCode_t VkDriver::Create(const se_GfxDriverCreateInfo_t& createInfo)
    {
        m_AllocatorInterface = createInfo.m_Allocator;

        // Create the instance
        VkApplicationInfo appInfo = Utils::k_SavannaDefaultVulkanAppInfo;
        VkInstanceCreateInfo instanceCreateInfo {};

        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pApplicationInfo = &appInfo;

        se_VkDriverCreateInfo_t* driverCreateInfo = reinterpret_cast<se_VkDriverCreateInfo_t*>(createInfo.m_pRealDriverCreateInfo);
        if (driverCreateInfo != nullptr)
        {
            instanceCreateInfo.enabledExtensionCount = driverCreateInfo->m_EnabledInstanceExtensionCount;
            instanceCreateInfo.ppEnabledExtensionNames = driverCreateInfo->m_ppEnabledInstanceExtensions;
            instanceCreateInfo.enabledLayerCount = driverCreateInfo->m_EnabledLayerCount;
            instanceCreateInfo.ppEnabledLayerNames = driverCreateInfo->m_ppEnabledLayers;
        }
        else
        {
            instanceCreateInfo.enabledExtensionCount = 0;
            instanceCreateInfo.ppEnabledExtensionNames = nullptr;
            instanceCreateInfo.enabledLayerCount = 0;
            instanceCreateInfo.ppEnabledLayerNames = nullptr;
        }

        VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &m_Instance);
        if (result != VK_SUCCESS)
        {
            return kSavannaGfxErrorCodeUnableToCreateGfxDriver;
        }

        // Create the device
        uint32_t physicalDeviceCount = 0;
        result = vkEnumeratePhysicalDevices(m_Instance, &physicalDeviceCount, nullptr);
        if (result != VK_SUCCESS)
        {
            return kSavannaGfxErrorCodeUnableToCreateGfxDriver;
        }

        // DynamicArray<VkPhysicalDevice> physicalDevices(physicalDeviceCount, );


        return kSavannaGfxErrorCodeSuccess;
    }

    se_GfxErrorCode_t VkDriver::Destroy()
    {
        return kSavannaGfxErrorCodeSuccess;
    }

} // namespace Savanna::Gfx::VK2


