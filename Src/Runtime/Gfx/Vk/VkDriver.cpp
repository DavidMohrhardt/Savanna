#include "VkDriver.h"

#include "SavannaVk2.h"

#include "VkAllocator.h"
#include "Utilities/VkDebugMessenger.h"
#include "Utilities/VkInfoCreateUtils.h"
#include "Utilities/VkExtensionUtils.h"
#include "Utilities/VkPhysicalDeviceSelectionUtils.h"
#include "Utilities/VkResultUtils.h"
#include "Utilities/VkSurfaceCreateUtils.h"

#define VK_DRIVER_SUCCESS_OR_TEARDOWN(__comparison, __msg) \
    if (!(__comparison))                            \
    {                                               \
        SAVANNA_LOG(__msg);                         \
        Teardown();                                 \
        return;                                     \
    }

namespace Savanna::Gfx::Vk2
{
    static VkDriver* g_pVulkanDriver = nullptr;

    se_AllocatorInterface_t VkDriver::s_AllocatorInterface = {};

    VkDriver::VkDriver(const se_GfxDriverCreateInfo_t& createInfo)
        : m_Instance(VK_NULL_HANDLE)
        , m_Gpu()
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
            // TODO Refactor once we support using temporary allocators in the managed memory arenas
            dynamic_array<const char*> enabledInstanceExtensions { driverCreateInfo.m_InstanceCreateArgs.m_EnabledInstanceExtensionCount, k_SavannaMemoryArenaIdGfx };
            dynamic_array<const char*> enabledInstanceLayers { driverCreateInfo.m_InstanceCreateArgs.m_EnabledLayerCount, k_SavannaMemoryArenaIdGfx };
            if (driverCreateInfo.m_EnableValidationLayers)
            {
                enabledInstanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
                enabledInstanceLayers.push_back("VK_LAYER_KHRONOS_validation");
            }

            if (driverCreateInfo.m_RequestSurface)
            {
                enabledInstanceExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
            }

            Utils::PopulateInstanceExtensions(enabledInstanceExtensions,
                driverCreateInfo.m_InstanceCreateArgs.m_ppEnabledInstanceExtensions,
                driverCreateInfo.m_InstanceCreateArgs.m_EnabledInstanceExtensionCount);

            for (uint32 i = 0; i < driverCreateInfo.m_InstanceCreateArgs.m_EnabledLayerCount; ++i)
            {
                enabledInstanceLayers.push_back(driverCreateInfo.m_InstanceCreateArgs.m_ppEnabledLayers[i]);
            }

            VkInstanceCreateInfo instanceCreateInfo {};
            Utils::PopulateInstanceCreateInfo(instanceCreateInfo,
                enabledInstanceExtensions.data(), enabledInstanceExtensions.size(),
                enabledInstanceLayers.data(), enabledInstanceLayers.size());
            instanceCreateInfo.pApplicationInfo = &appInfo;

            VK_MUST_SUCCEED(vkCreateInstance(&instanceCreateInfo, VkAllocator::Get(), &m_Instance), "Failed to create Vulkan instance.");
        }

        if (driverCreateInfo.m_EnableValidationLayers)
        {
            Utils::DebugMessenger::Initialize(m_Instance, VkAllocator::Get());
        }

        if (driverCreateInfo.m_RequestSurface && driverCreateInfo.m_pWindowHandle != nullptr)
        {
            VK_DRIVER_SUCCESS_OR_TEARDOWN(
                (m_Surface = Utils::CreateSurface(m_Instance, driverCreateInfo.m_pWindowHandle, VkAllocator::Get())) != VK_NULL_HANDLE,
                "Failed to create surface.");
        }

        // Initialize the GPU
        VK_DRIVER_SUCCESS_OR_TEARDOWN(m_Gpu.TryInitialize(driverCreateInfo, m_Instance, m_Surface, VkAllocator::Get()), "Failed to initialize GPU.");
    }

    VkDriver::~VkDriver()
    {
        Teardown();
    }

    se_GfxErrorCode_t VkDriver::Initialize(const se_GfxDriverCreateInfo_t &createInfo)
    {
        if (!IsAllocatorInterfaceValid(createInfo.m_pAllocationInterface))
        {
            return kSavannaGfxErrorCodeInvalidAllocatorInterface;
        }

        VkAllocator::SetVkAllocationInterfacePtr(createInfo.m_pAllocationInterface);
        s_AllocatorInterface = *createInfo.m_pAllocationInterface;

        InterfaceAllocator allocator{ s_AllocatorInterface };
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
        InterfaceAllocator allocator{ s_AllocatorInterface };
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

    se_GfxErrorCode_t VkDriver::CreateSwapchain(const se_GfxSwapchainCreateInfo_t &createInfo, se_GfxHandle_t *const pOutSwapchainHandle)
    {
        se_GfxErrorCode_t result = kSavannaGfxErrorCodeNotInitialized;
        if (g_pVulkanDriver != nullptr)
        {
            result = g_pVulkanDriver->m_Swapchain.Initialize(
                createInfo,
                g_pVulkanDriver->m_Gpu,
                g_pVulkanDriver->m_Surface);

            if (SAVANNA_GFX_SUCCESS(result) && pOutSwapchainHandle != nullptr)
            {
                *pOutSwapchainHandle = (se_GfxHandle_t)&g_pVulkanDriver->m_Swapchain;
            }
        }

        return result;
    }

    void VkDriver::PopulateDriverInterface(se_GfxDriverInterface_t &outDriverInterface)
    {

#   define FILL_OUT_INTERFACE_FUNC(__func) \
        .m_pfn##__func = VkDriver::__func

        constexpr se_GfxDriverInterface_t k_VulkanDriverInterface
        {
            FILL_OUT_INTERFACE_FUNC(Initialize),
            FILL_OUT_INTERFACE_FUNC(Destroy),
            FILL_OUT_INTERFACE_FUNC(GetDriverHandle),
            FILL_OUT_INTERFACE_FUNC(CreateSwapchain),
            .m_pfnGetBackend = []() { return kSavannaGfxApiVulkan; },
        };
        outDriverInterface = k_VulkanDriverInterface;

#   undef FILL_OUT_INTERFACE_FUNC
    }

    void VkDriver::Teardown()
    {
        m_Gpu.Reset(m_Instance, VkAllocator::Get());

        if (m_Surface != VK_NULL_HANDLE)
        {
            vkDestroySurfaceKHR(m_Instance, m_Surface, VkAllocator::Get());
            m_Surface = VK_NULL_HANDLE;
        }

        // Okay to call even if the debug utils messenger was never created
        Utils::DebugMessenger::Destroy();

        if (m_Instance != VK_NULL_HANDLE)
        {
            vkDestroyInstance(m_Instance, VkAllocator::Get());
            m_Instance = VK_NULL_HANDLE;
        }
    }

} // namespace Savanna::Gfx::Vk2
