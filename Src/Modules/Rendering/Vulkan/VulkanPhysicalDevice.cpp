#include "VulkanPhysicalDevice.h"

#include <cstdlib>
#include <mutex>
#include <cassert>
#include <vector>
#include <stdexcept>

#include <Types/Exceptions/SavannaException.h>
#include <Types/Strings/FixedString.h>

// TODO: Rework this to just have each PhysicalDevice produce it's descriptor and then init that.

namespace Savanna::Rendering::Vulkan
{
    inline VkPhysicalDeviceProperties GetDeviceProperties(VkPhysicalDevice& device, VkPhysicalDeviceProperties& deviceProperties)
    {
        vkGetPhysicalDeviceProperties(device, &deviceProperties);

        return deviceProperties;
    }

#if SAVANNA_VULKAN_ENABLE_DEBUGGING
    inline void PrintDeviceProperties(VkPhysicalDeviceProperties& deviceProperties)
    {
        FixedString512 propertiesString = FixedString256("Vulkan Physical Device\n\t");
        // TODO
    }
#endif

    bool HasGraphicsQueueFamily(VkPhysicalDevice physicalDevice)
    {
        uint32 queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

        for (const auto& queueFamily : queueFamilies)
        {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                // outDesc = { queueFamily.queueFlags };
                return true;
            }
        }

        return false;
    }

    inline bool TryGetSuitableDevices(VkPhysicalDevice* pDevices, uint32 deviceCount, VkPhysicalDevice& outDevice)
    {
        for (int i = 0; i < deviceCount; i++)
        {
            VkPhysicalDeviceProperties deviceProperties;
            GetDeviceProperties(pDevices[i], deviceProperties);

#if SAVANNA_VULKAN_ENABLE_DEBUGGING
            PrintDeviceProperties(deviceProperties);
#endif

            if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && HasGraphicsQueueFamily(pDevices[i]))
            {
                outDevice = pDevices[i];
                return true;
            }
        }

        return false;
    }

    VulkanPhysicalDevice::VulkanPhysicalDevice(VkInstance instance)
        : m_VulkanPhysicalDevice(VK_NULL_HANDLE)
    {
        SAVANNA_INSERT_SCOPED_PROFILER("VulkanPhysicalDevice::ctor");
        uint32 deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        if (deviceCount == 0)
        {
            throw Savanna::RuntimeErrorException("No Physical Devices Found.");
        }

        VkPhysicalDevice* pDevices = reinterpret_cast<VkPhysicalDevice*>(malloc(sizeof(VkPhysicalDevice) * deviceCount));
        vkEnumeratePhysicalDevices(instance, &deviceCount, pDevices);

        bool result = TryGetSuitableDevices(pDevices, deviceCount, m_VulkanPhysicalDevice);

        free(pDevices);

        if (m_VulkanPhysicalDevice == VK_NULL_HANDLE)
        {
            throw Savanna::RuntimeErrorException("failed to find a suitable GPU!");
        }
    }

    VulkanPhysicalDevice::~VulkanPhysicalDevice() {}
} // namespace Savanna::Rendering::Vulkan
