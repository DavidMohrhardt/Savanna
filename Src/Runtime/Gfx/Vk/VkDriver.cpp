#include "VkDriver.h"

#include "SavannaVk2.h"

#include "VkAllocator.h"
#include "Utilities/VkDebugMessenger.h"
#include "Utilities/VkInfoCreateUtils.h"
#include "Utilities/VkExtensionUtils.h"
#include "Utilities/VkPhysicalDeviceSelectionUtils.h"
#include "Utilities/VkResultUtils.h"
#include "Utilities/VkSurfaceCreateUtils.h"

#define VK_DRIVER_SUCCESS_OR_TEARDOWN(__comparison, __errCode,__msg) \
    if (!(__comparison))                            \
    {                                               \
        SAVANNA_LOG(__msg);                         \
        Teardown();                                 \
        outResult = __errCode;                      \
        return;                                     \
    }

namespace Savanna::Gfx::Vk2
{
    static VkDriver* g_pVulkanDriver = nullptr;

    se_AllocatorInterface_t VkDriver::s_AllocatorInterface = {};

    VkDriver::VkDriver(const se_GfxDriverCreateInfo_t& createInfo, GfxErrorCode& outResult)
        : m_Instance(VK_NULL_HANDLE)
        , m_Gpu()
    {
        // Create the Vulkan Instance
        se_VkDriverCreateInfo_t driverCreateInfo = createInfo.m_pNext == nullptr
            ? se_VkDriverCreateInfo_t{}
            : *reinterpret_cast<se_VkDriverCreateInfo_t*>(createInfo.m_pNext);

        outResult = CreateInstance(createInfo, driverCreateInfo);
        if (SAVANNA_GFX_FAILURE(outResult))
        {
            return;
        }

        if (driverCreateInfo.m_RequestSurface && driverCreateInfo.m_pWindowHandle != nullptr)
        {
            m_Surface = Utils::CreateSurface(m_Instance, driverCreateInfo.m_pWindowHandle, VkAllocator::Get());
            VK_DRIVER_SUCCESS_OR_TEARDOWN(
                m_Surface != VK_NULL_HANDLE,
                kSavannaGfxErrorCodeUnableToCreateGfxDriver,
                "Failed to create surface.");
        }

        // Initialize the GPU
        VK_DRIVER_SUCCESS_OR_TEARDOWN(
            m_Gpu.TryInitialize(driverCreateInfo, m_Instance, m_Surface, VkAllocator::Get()),
            kSavannaGfxErrorCodeUnableToCreateGfxDriver,
            "Failed to initialize GPU.");
    }

    VkDriver::~VkDriver()
    {
        Teardown();
    }

    se_GfxErrorCode_t VkDriver::Initialize(const se_GfxDriverCreateInfo_t &createInfo)
    {
        GfxErrorCode result = kSavannaGfxErrorCodeSuccess;
        if (!IsAllocatorInterfaceValid(createInfo.m_pAllocationInterface))
        {
            return kSavannaGfxErrorCodeInvalidAllocatorInterface;
        }

        VkAllocator::SetVkAllocationInterfacePtr(createInfo.m_pAllocationInterface);

        InterfaceAllocator allocator{ VkAllocator::GetAllocatorInterface() };
        VkDriver* pDriver = allocator.New<VkDriver>(createInfo, result);
        if (SAVANNA_GFX_FAILURE(result)) SAVANNA_BRANCH_UNLIKELY
        {
            allocator.Delete(pDriver);
        }
        else SAVANNA_BRANCH_LIKELY
        {
            g_pVulkanDriver = pDriver;
        }

        return result;
    }

    se_GfxErrorCode_t VkDriver::Destroy()
    {
        InterfaceAllocator allocator{ VkAllocator::GetAllocatorInterface() };
        if (g_pVulkanDriver != nullptr)
        {
            allocator.Delete(g_pVulkanDriver);
            g_pVulkanDriver = nullptr;
            VkAllocator::SetVkAllocationInterfacePtr(nullptr);
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

    se_GfxErrorCode_t VkDriver::CreateShaderModule(
        const se_GfxShaderCreateInfo_t& createInfo,
        se_GfxShaderHandle_t& outShaderModuleHandle)
    {
        outShaderModuleHandle = k_SavannaGfxInvalidShaderModuleHandle;
        if (g_pVulkanDriver != nullptr)
        {
            outShaderModuleHandle = g_pVulkanDriver->m_ShaderModuleCache.CreateShaderModuleSynchronized(g_pVulkanDriver->m_Gpu, createInfo);
        }

        return outShaderModuleHandle != k_SavannaGfxInvalidShaderModuleHandle
            ? kSavannaGfxErrorCodeSuccess
            : kSavannaGfxErrorCodeUnknownError;
    }

    se_JobHandle_t VkDriver::CreateShaderModulesAsync(
        const se_GfxShaderCreateInfo_t* pCreateInfos,
        const size_t createInfoCount,
        se_GfxShaderHandle_t** const ppOutShaderModuleHandles)
    {
        JobHandle jobHandle = k_InvalidJobHandle;
        if (g_pVulkanDriver != nullptr)
        {
            jobHandle = g_pVulkanDriver->m_ShaderModuleCache.CreateShaderModulesAsync(
                g_pVulkanDriver->m_Gpu,
                const_cast<se_GfxShaderCreateInfo_t*>(pCreateInfos),
                createInfoCount,
                ppOutShaderModuleHandles);
        }

        return jobHandle;
    }

    GfxErrorCode VkDriver::CreateInstance(
        const se_GfxDriverCreateInfo_t& createInfo,
        se_VkDriverCreateInfo_t& driverCreateInfo)
    {
        VkApplicationInfo appInfo = Utils::k_SavannaDefaultVulkanAppInfo;
        appInfo.pApplicationName = createInfo.m_pApplicationName == nullptr
            ? "Savanna"
            : createInfo.m_pApplicationName;

        se_VkInstanceCreateArgs_t& instanceCreateArgs = driverCreateInfo.m_InstanceCreateArgs;
        dynamic_array<const char*> enabledInstanceExtensions {
            instanceCreateArgs.m_EnabledInstanceExtensionCount,
            kSavannaAllocatorKindTemp
        };

        dynamic_array<const char*> enabledInstanceLayers {
            instanceCreateArgs.m_EnabledLayerCount,
            kSavannaAllocatorKindTemp
        };

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
            instanceCreateArgs.m_ppEnabledInstanceExtensions,
            instanceCreateArgs.m_EnabledInstanceExtensionCount);

        for (uint32 i = 0; i < instanceCreateArgs.m_EnabledLayerCount; ++i)
        {
            enabledInstanceLayers.push_back(instanceCreateArgs.m_ppEnabledLayers[i]);
        }

        VkInstanceCreateInfo instanceCreateInfo {};
        Utils::PopulateInstanceCreateInfo(instanceCreateInfo,
            enabledInstanceExtensions.data(), enabledInstanceExtensions.size(),
            enabledInstanceLayers.data(), enabledInstanceLayers.size());
        instanceCreateInfo.pApplicationInfo = &appInfo;

        VkResult result = vkCreateInstance(&instanceCreateInfo, VkAllocator::Get(), &m_Instance);

        if (VK_SUCCEEDED(result) && driverCreateInfo.m_EnableValidationLayers)
        {
            Utils::DebugMessenger::Initialize(m_Instance, VkAllocator::Get());
        }

        return ResultToErrorCode(result);
    }

    void VkDriver::PopulateDriverInterface(
        se_GfxDriverInterface_t &outDriverInterface) {
#define FILL_OUT_INTERFACE_FUNC(__func) .m_pfn##__func = VkDriver::__func

        constexpr se_GfxDriverInterface_t k_VulkanDriverInterface
        {
            FILL_OUT_INTERFACE_FUNC(Initialize),
            FILL_OUT_INTERFACE_FUNC(Destroy),
            FILL_OUT_INTERFACE_FUNC(GetDriverHandle),
            FILL_OUT_INTERFACE_FUNC(CreateSwapchain),
            FILL_OUT_INTERFACE_FUNC(CreateShaderModule),
            FILL_OUT_INTERFACE_FUNC(CreateShaderModulesAsync),
            .m_pfnGetBackend = []() { return kSavannaGfxApiVulkan; },
        };
        outDriverInterface = k_VulkanDriverInterface;

#   undef FILL_OUT_INTERFACE_FUNC
    }

    void VkDriver::Teardown()
    {
        m_ShaderModuleCache.Clear(m_Gpu);

        if (m_Swapchain.m_Swapchain != VK_NULL_HANDLE)
        {
            m_Swapchain.Destroy(m_Gpu);
        }

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
