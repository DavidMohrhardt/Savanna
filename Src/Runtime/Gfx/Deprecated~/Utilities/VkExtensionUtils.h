#pragma once

#include "SavannaVk.h"

namespace Savanna::Gfx::Vk::Utils
{
    inline static bool IsExtensionSupported(const char *extensionName, const std::vector<VkExtensionProperties>& supportedExtensions)
    {
        for (const auto& extension : supportedExtensions)
        {
            if (strcmp(extension.extensionName, extensionName) == 0)
            {
                return true;
            }
        }
        return false;
    }

    inline void ValidateInstanceExtensions(const char* const* ppExtensions, uint32 extensionCount)
    {
        uint32 supportedExtensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &supportedExtensionCount, nullptr);
        std::vector<VkExtensionProperties> supportedExtensions(supportedExtensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &supportedExtensionCount, supportedExtensions.data());
        bool result = true;
        for (uint32 i = 0; i < extensionCount; ++i)
        {
            if (!IsExtensionSupported(ppExtensions[i], supportedExtensions))
            {
                SAVANNA_FATAL_LOG("Extension requested extension {} is not supported.", ppExtensions[i]);
                result = false;
            }
        }

        if (!result)
        {
            throw Savanna::RuntimeErrorException("Extension requested extension is not supported");
        }
    }

    inline void ValidateDeviceExtensions(const VkPhysicalDevice& physicalDevice, const char* const* ppExtensions, uint32 extensionCount)
    {
        uint32 supportedExtensionCount = 0;
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &supportedExtensionCount, nullptr);
        std::vector<VkExtensionProperties> supportedExtensions(supportedExtensionCount);
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &supportedExtensionCount, supportedExtensions.data());

        for (uint32 i = 0; i < extensionCount; ++i)
        {
            if (!IsExtensionSupported(ppExtensions[i], supportedExtensions))
            {
                throw Savanna::RuntimeErrorException("Extension requested extension is not supported");
            }
        }
    }
}
