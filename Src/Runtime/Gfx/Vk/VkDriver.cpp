#include "VkDriver.h"

#include "SavannaVk2.h"

#include "VkAllocator.h"
#include "Utilities/VkInfoCreateUtils.h"
#include "Utilities/VkPhysicalDeviceSelectionUtils.h"

namespace Savanna::Gfx::Vk2
{
    static VkDriver* g_pVulkanDriver = nullptr;

    VkDriver::VkDriver(const se_GfxDriverCreateInfo_t& createInfo)
        : m_Instance(VK_NULL_HANDLE)
        , m_PhysicalDevice(VK_NULL_HANDLE)
        , m_LogicalDevice(VK_NULL_HANDLE)
        , m_Allocator(createInfo.m_Allocator)
        , m_AllocationCallbacks(VkAllocator::CreateAllocationCallbacksForInterface(&m_Allocator))
    {
        // Create the Vulkan Instance
        VkApplicationInfo appInfo = Utils::k_SavannaDefaultVulkanAppInfo;
        appInfo.pApplicationName = createInfo.m_pApplicationName == nullptr
            ? "Savanna"
            : createInfo.m_pApplicationName;

        se_VkDriverCreateInfo_t driverCreateInfo = createInfo.m_pNext == nullptr
            ? se_VkDriverCreateInfo_t{}
            : *reinterpret_cast<se_VkDriverCreateInfo_t*>(createInfo.m_pNext);

        VkInstanceCreateInfo instanceCreateInfo {};
        Utils::PopulateInstanceCreateInfo(&driverCreateInfo, instanceCreateInfo);
        instanceCreateInfo.pApplicationInfo = &appInfo;

        VK_MUST_SUCCEED(vkCreateInstance(&instanceCreateInfo, &m_AllocationCallbacks, &m_Instance), "Failed to create Vulkan instance.");

        // Get the physical device
        if (!Utils::TrySelectPhysicalDevice(m_Instance, m_PhysicalDevice, driverCreateInfo.m_PhysicalDeviceCreateArgs, createInfo.m_Allocator))
        {
            SAVANNA_LOG("Failed to select a physical device.");
            return;
        }

        // Print the physical device properties
        vkGetPhysicalDeviceProperties(m_PhysicalDevice, &m_PhysicalDeviceProperties);
        SAVANNA_LOG("Selected physical device:\n\t{}", m_PhysicalDeviceProperties.deviceName);

        // Create the logical device
        // VkDeviceCreateInfo logicalDeviceCreateInfo = Utils::PopulateLogicalDeviceCreateInfo(&driverCreateInfo);
        // VkPhysicalDeviceFeatures physicalDeviceFeatures;
        // vkGetPhysicalDeviceFeatures(m_PhysicalDevice, &physicalDeviceFeatures);
        // logicalDeviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;

        // VK_MUST_SUCCEED(vkCreateDevice(m_PhysicalDevice, &logicalDeviceCreateInfo, &m_AllocationCallbacks, &m_LogicalDevice), "Failed to create logical device.");
    }

    VkDriver::~VkDriver()
    {
        if (m_LogicalDevice != VK_NULL_HANDLE)
        {
            vkDestroyDevice(m_LogicalDevice, &m_AllocationCallbacks);
            m_LogicalDevice = VK_NULL_HANDLE;
        }

        if (m_Instance != VK_NULL_HANDLE)
        {
            vkDestroyInstance(m_Instance, &m_AllocationCallbacks);
            m_Instance = VK_NULL_HANDLE;
        }
    }

    se_GfxErrorCode_t VkDriver::Initialize(const se_GfxDriverCreateInfo_t &createInfo)
    {
        if (!IsAllocatorInterfaceValid(createInfo.m_Allocator))
        {
            return kSavannaGfxErrorCodeInvalidAllocatorInterface;
        }

        InterfaceAllocator allocator{createInfo.m_Allocator};
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
        if (g_pVulkanDriver != nullptr)
        {
            g_pVulkanDriver->m_Allocator.Delete(g_pVulkanDriver);
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
