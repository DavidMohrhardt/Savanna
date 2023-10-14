#include "VkDriver.h"

#include "SavannaVk2.h"

#include "VkAllocator.h"
#include "Utilities/VkInfoCreateUtils.h"
#include "Utilities/VkExtensionUtils.h"
#include "Utilities/VkPhysicalDeviceSelectionUtils.h"
#include "Utilities/VkResultUtils.h"

namespace Savanna::Gfx::Vk2
{
    static VkDriver* g_pVulkanDriver = nullptr;

    VkDriver::VkDriver(const se_GfxDriverCreateInfo_t& createInfo)
        : m_Instance(VK_NULL_HANDLE)
        , m_Gpu()
        , m_AllocatorInterface(createInfo.m_AllocatorInterface)
        , m_AllocationCallbacks(VkAllocator::CreateAllocationCallbacksForInterface(&m_AllocatorInterface))
    {
        // Create the Vulkan Instance
        VkApplicationInfo appInfo = Utils::k_SavannaDefaultVulkanAppInfo;
        appInfo.pApplicationName = createInfo.m_pApplicationName == nullptr
            ? "Savanna"
            : createInfo.m_pApplicationName;

        se_VkDriverCreateInfo_t driverCreateInfo = createInfo.m_pNext == nullptr
            ? se_VkDriverCreateInfo_t{}
            : *reinterpret_cast<se_VkDriverCreateInfo_t*>(createInfo.m_pNext);

        {
            // TODO make this use a temporary allocator as it's not needed after this function
            DynamicArray<const char*> enabledInstanceExtensions { driverCreateInfo.m_InstanceCreateArgs.m_EnabledInstanceExtensionCount, createInfo.m_AllocatorInterface };
            DynamicArray<const char*> enabledInstanceLayers { driverCreateInfo.m_InstanceCreateArgs.m_EnabledLayerCount, createInfo.m_AllocatorInterface };
            if (driverCreateInfo.m_EnableValidationLayers)
            {
                enabledInstanceExtensions.Append(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
                enabledInstanceLayers.Append("VK_LAYER_KHRONOS_validation");
            }

            if (driverCreateInfo.m_EnableSurfaceExtension)
            {
                enabledInstanceExtensions.Append(VK_KHR_SURFACE_EXTENSION_NAME);
            }

            for (uint32 i = 0; i < driverCreateInfo.m_InstanceCreateArgs.m_EnabledInstanceExtensionCount; ++i)
            {
                enabledInstanceExtensions.Append(driverCreateInfo.m_InstanceCreateArgs.m_ppEnabledInstanceExtensions[i]);
            }

            for (uint32 i = 0; i < driverCreateInfo.m_InstanceCreateArgs.m_EnabledLayerCount; ++i)
            {
                enabledInstanceLayers.Append(driverCreateInfo.m_InstanceCreateArgs.m_ppEnabledLayers[i]);
            }

            Utils::ValidateInstanceExtensions(enabledInstanceExtensions.Data(), enabledInstanceExtensions.Size());

            VkInstanceCreateInfo instanceCreateInfo {};
            Utils::PopulateInstanceCreateInfo(instanceCreateInfo,
                enabledInstanceExtensions.data(), enabledInstanceExtensions.size(),
                enabledInstanceLayers.data(), enabledInstanceLayers.size());
            instanceCreateInfo.pApplicationInfo = &appInfo;

            auto result = vkCreateInstance(&instanceCreateInfo, nullptr, &m_Instance);
            if (result != VK_SUCCESS)
            {
                SAVANNA_LOG("Failed to create Vulkan instance: {}", ResultToString(result));
                return;
            }
        }

        // Get the physical device
        if (!Utils::TrySelectPhysicalDevice(m_Instance, m_Gpu.m_PhysicalDevice, driverCreateInfo.m_PhysicalDeviceCreateArgs, createInfo.m_AllocatorInterface))
        {
            SAVANNA_LOG("Failed to select a physical device.");
            return;
        }

        // Print the physical device properties
        vkGetPhysicalDeviceProperties(m_Gpu.m_PhysicalDevice, &m_Gpu.m_PhysicalDeviceProperties);
        SAVANNA_LOG("Selected physical device:\n\t{}", m_Gpu.m_PhysicalDeviceProperties.deviceName);

        // Create the logical device
        // VkDeviceCreateInfo logicalDeviceCreateInfo = Utils::PopulateLogicalDeviceCreateInfo(&driverCreateInfo);
        // VkPhysicalDeviceFeatures physicalDeviceFeatures;
        // vkGetPhysicalDeviceFeatures(m_PhysicalDevice, &physicalDeviceFeatures);
        // logicalDeviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;

        // VK_MUST_SUCCEED(vkCreateDevice(m_PhysicalDevice, &logicalDeviceCreateInfo, &m_AllocationCallbacks, &m_LogicalDevice), "Failed to create logical device.");
    }

    VkDriver::~VkDriver()
    {
        m_Gpu.Reset(m_Instance);

        if (m_Instance != VK_NULL_HANDLE)
        {
            vkDestroyInstance(m_Instance, nullptr);
            m_Instance = VK_NULL_HANDLE;
        }
    }

    se_GfxErrorCode_t VkDriver::Initialize(const se_GfxDriverCreateInfo_t &createInfo)
    {
        if (!IsAllocatorInterfaceValid(createInfo.m_AllocatorInterface))
        {
            return kSavannaGfxErrorCodeInvalidAllocatorInterface;
        }

        InterfaceAllocator allocator{createInfo.m_AllocatorInterface};
        VkDriver* pDriver = allocator.New<VkDriver>(createInfo);
        if (pDriver->m_Instance == VK_NULL_HANDLE)
        {
            allocator.Delete(pDriver);
            return kSavannaGfxErrorCodeUnableToCreateGfxDriver;
        }

        g_pVulkanDriver = pDriver;
        return kSavannaGfxErrorCodeSuccess;
    }

    se_GfxErrorCode_t VkDriver::Destroy()
    {
        InterfaceAllocator allocator{g_pVulkanDriver->m_AllocatorInterface};
        if (g_pVulkanDriver != nullptr)
        {
            allocator.Delete(g_pVulkanDriver);
            g_pVulkanDriver = nullptr;
            return kSavannaGfxErrorCodeSuccess;
        }

        return kSavannaGfxErrorCodeNotInitialized;
    }

    se_GfxDriverHandle_t VkDriver::GetDriverHandle()
    {
        return reinterpret_cast<se_GfxDriverHandle_t>(g_pVulkanDriver);
    }

    void VkDriver::PopulateDriverInterface(se_GfxDriverInterface_t &outDriverInterface)
    {
        constexpr se_GfxDriverInterface_t k_VulkanDriverInterface
        {
            .m_pfnInitialize = VkDriver::Initialize,
            .m_pfnDestroy = VkDriver::Destroy,
            .m_pfnGetDriverHandle = VkDriver::GetDriverHandle,
            .m_pfnGetBackend = []() { return kSavannaGfxApiVulkan; },
        };
        outDriverInterface = k_VulkanDriverInterface;
    }

} // namespace Savanna::Gfx::Vk2
