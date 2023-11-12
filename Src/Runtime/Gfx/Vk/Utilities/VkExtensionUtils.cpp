#include "VkExtensionUtils.h"

#include "Utilities/SavannaCoding.h"

namespace Savanna::Gfx::Vk2::Utils
{
    inline bool IsExtensionSupported(const char *extensionName, const dynamic_array<VkExtensionProperties>& supportedExtensions)
    {
        static auto s_Sentinel = [=]()
        {
            SAVANNA_LOG("Savanna Vulkan Supported Extensions:");
            for (auto& extension : supportedExtensions)
            {
                SAVANNA_LOG("\t{}", extension.extensionName);
            }
            return 0;
        }();

        for (const auto& extension : supportedExtensions)
        {
            if (strcmp(extension.extensionName, extensionName) == 0)
            {
                return true;
            }
        }
        SAVANNA_LOG("Extension {} is not supported.", extensionName);
        return false;
    }

    inline void PopulateExtensionBuffer(
        dynamic_array<const char *> &outExtensions,
        const char *const *ppExtensions,
        uint32 extensionCount)
    {
        if (extensionCount == 0)
            return;
        if (ppExtensions == nullptr)
            return;

        for (uint32 i = 0; i < extensionCount; ++i)
        {
            bool found = false;
            for (uint32 j = 0; j < outExtensions.size(); ++j)
            {
                if (strcmp(ppExtensions[i], outExtensions[j]) == 0)
                {
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                outExtensions.push_back(ppExtensions[i]);
            }
        }
    }

    void ValidateInstanceExtensions(const char* const* ppExtensions, uint32 extensionCount)
    {
        uint32 supportedExtensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &supportedExtensionCount, nullptr);
        dynamic_array<VkExtensionProperties> supportedExtensions(supportedExtensionCount, kSavannaAllocatorKindTemp);
        supportedExtensions.resize_uninitialized(supportedExtensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &supportedExtensionCount, supportedExtensions.data());

#if SAVANNA_LOG_LEVEL >= SAVANNA_LOG_LEVEL_DEBUG
        SAVANNA_DEBUG_LOG("Savanna Vulkan Supported Instance Extensions:");
        for (auto& extension : supportedExtensions)
        {
            SAVANNA_DEBUG_LOG("\t{}", extension.extensionName);
        }
#endif

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

    void ValidateDeviceExtensions(const VkPhysicalDevice& physicalDevice, const char* const* ppExtensions, uint32 extensionCount)
    {
        uint32 supportedExtensionCount = 0;
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &supportedExtensionCount, nullptr);
        dynamic_array<VkExtensionProperties> supportedExtensions(supportedExtensionCount, kSavannaAllocatorKindTemp);
        supportedExtensions.resize_uninitialized(supportedExtensionCount);
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &supportedExtensionCount, supportedExtensions.data());

#if SAVANNA_LOG_LEVEL >= SAVANNA_LOG_LEVEL_DEBUG
        SAVANNA_DEBUG_LOG("Savanna Vulkan Supported Device Extensions:");
        for (auto& extension : supportedExtensions)
        {
            SAVANNA_DEBUG_LOG("\t{}", extension.extensionName);
        }
#endif

        for (uint32 i = 0; i < extensionCount; ++i)
        {
            if (!IsExtensionSupported(ppExtensions[i], supportedExtensions))
            {
                throw Savanna::RuntimeErrorException("Extension requested extension is not supported");
            }
        }
    }

    void PopulateInstanceExtensions(
        dynamic_array<const char*>& outExtensions,
        const char* const* ppExtensions,
        uint32 extensionCount)
    {
        PopulateExtensionBuffer(outExtensions, ppExtensions, extensionCount);
        ValidateInstanceExtensions(outExtensions.data(), outExtensions.size());
    }

    void PopulateDeviceExtensions(
        dynamic_array<const char*>& outExtensions,
        VkPhysicalDevice physicalDevice,
        const char* const* ppExtensions,
        uint32 extensionCount)
    {
        PopulateExtensionBuffer(outExtensions, ppExtensions, extensionCount);
        ValidateDeviceExtensions(physicalDevice, outExtensions.data(), outExtensions.size());
    }
} // namespace Savanna::Gfx::Vk2::Utils
