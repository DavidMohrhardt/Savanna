#include "VkDriver.h"

#include "SavannaVk2.h"

#include "VkAllocator.h"
#include "Utilities/VkInfoCreateUtils.h"

namespace Savanna::Gfx::Vk2
{
    se_GfxErrorCode_t VkDriver::Create(const se_GfxDriverCreateInfo_t& createInfo)
    {
        m_Allocator = VkAllocator{
            createInfo.m_Allocator,
            createInfo.m_pUserData
        };

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

        VkResult result = vkCreateInstance(
            &instanceCreateInfo, &m_Allocator.GetAllocationCallbacks(), &m_Instance);
        if (result != VK_SUCCESS)
        {
            return kSavannaGfxErrorCodeUnableToCreateGfxDriver;
        }

        // Process the device characteristics

        // Enumerate the physical devices
        // uint32_t physicalDeviceCount = 0;
        // result = vkEnumeratePhysicalDevices(m_Instance, &physicalDeviceCount, nullptr);
        // DynamicArray<VkPhysicalDevice> physicalDevices(physicalDeviceCount, m_Allocator.m_Allocator.GetInterface());
        // physicalDevices.Resize(physicalDeviceCount);
        // result = vkEnumeratePhysicalDevices(m_Instance, &physicalDeviceCount, physicalDevices.Data());
        // if (result != VK_SUCCESS)
        // {
        //     return kSavannaGfxErrorCodeUnableToCreateGfxDriver;
        // }

        // // TODO @DavidMohrhardt: Handle multiple physical devices.
        // // For now just grab the first physical device.
        // m_PhysicalDevice = physicalDevices[0];

        // // Get the devices capabilities
        // VkPhysicalDeviceProperties physicalDeviceProperties;
        // vkGetPhysicalDeviceProperties(m_PhysicalDevice, &physicalDeviceProperties);

        // VkPhysicalDeviceFeatures physicalDeviceFeatures;
        // vkGetPhysicalDeviceFeatures(m_PhysicalDevice, &physicalDeviceFeatures);

        return kSavannaGfxErrorCodeSuccess;
    }

    se_GfxErrorCode_t VkDriver::Destroy()
    {
        if (m_LogicalDevice != VK_NULL_HANDLE)
        {
            vkDestroyDevice(m_LogicalDevice, &m_Allocator.GetAllocationCallbacks());
            m_LogicalDevice = VK_NULL_HANDLE;
        }

        if (m_Instance != VK_NULL_HANDLE)
        {
            vkDestroyInstance(m_Instance, &m_Allocator.GetAllocationCallbacks());
            m_Instance = VK_NULL_HANDLE;
        }

        return kSavannaGfxErrorCodeSuccess;
    }

} // namespace Savanna::Gfx::VK2


