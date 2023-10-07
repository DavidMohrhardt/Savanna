#include "VkDriver.h"

#include "SavannaVk2.h"

#include "VkAllocator.h"
#include "Utilities/VkInfoCreateUtils.h"

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

        VkInstanceCreateInfo instanceCreateInfo = Utils::PopulateInstanceCreateInfo(&driverCreateInfo);
        instanceCreateInfo.pApplicationInfo = &appInfo;

        VkResult result = vkCreateInstance(&instanceCreateInfo, &m_AllocationCallbacks, &m_Instance);
        if (result != VK_SUCCESS)
        {
            m_Instance = VK_NULL_HANDLE;
            return;
        }
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
        outDriverInterface.m_pfnInitialize = VkDriver::Initialize;
        outDriverInterface.m_pfnDestroy = VkDriver::Destroy;
        outDriverInterface.m_pfnGetDriverHandle = VkDriver::GetDriverHandle;
        outDriverInterface.m_pfnGetBackend = []() { return kSavannaGfxApiVulkan; };
    }

} // namespace Savanna::Gfx::Vk2
