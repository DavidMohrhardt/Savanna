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

namespace savanna::gfx::vk
{
    static VkDriver* g_pVulkanDriver = nullptr;

    seAllocatorInterface VkDriver::s_AllocatorInterface = {};

    VkDriver::VkDriver(const seGfxDriverCreateInfo& createInfo, GfxErrorCode& outResult)
        : m_Instance(VK_NULL_HANDLE)
        , m_Gpu()
    {
        // Create the Vulkan Instance
        seVkDriverCreateInfo driverCreateInfo = createInfo.m_pNext == nullptr
            ? seVkDriverCreateInfo{}
            : *reinterpret_cast<seVkDriverCreateInfo*>(createInfo.m_pNext);

        outResult = CreateInstance(createInfo, driverCreateInfo);
        if (SAVANNA_GFX_FAILURE(outResult))
        {
            return;
        }

        LoadLibraryPhase1(m_Instance);

        if (driverCreateInfo.m_RequestSurface && driverCreateInfo.m_pWindowHandle != nullptr)
        {
            m_Surface = utils::CreateSurface(m_Instance, driverCreateInfo.m_pWindowHandle, VkAllocator::Get());
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

    seGfxErrorCode VkDriver::Initialize(const seGfxDriverCreateInfo &createInfo)
    {
        GfxErrorCode result = kSavannaGfxErrorCodeSuccess;
        if (!IsAllocatorInterfaceValid(createInfo.m_pAllocationInterface))
        {
            return kSavannaGfxErrorCodeInvalidAllocatorInterface;
        }

        if (!TryLoadVulkanLibrary())
        {
            return kSavannaGfxErrorCodeUnknownError;
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

    seGfxErrorCode VkDriver::Destroy()
    {
        InterfaceAllocator allocator{ VkAllocator::GetAllocatorInterface() };
        if (g_pVulkanDriver != nullptr)
        {
            allocator.Delete(g_pVulkanDriver);
            g_pVulkanDriver = nullptr;
            VkAllocator::SetVkAllocationInterfacePtr(nullptr);

            UnloadVulkanLibrary();
            return kSavannaGfxErrorCodeSuccess;
        }

        return kSavannaGfxErrorCodeNotInitialized;
    }

    seGfxDriver VkDriver::GetDriver()
    {
        return reinterpret_cast<seGfxDriver>(g_pVulkanDriver);
    }

    seGfxErrorCode VkDriver::CreateSwapchain(const seGfxSwapchainCreateInfo &createInfo, seGfxHandle *const pOutSwapchainHandle)
    {
        seGfxErrorCode result = kSavannaGfxErrorCodeNotInitialized;
        if (g_pVulkanDriver != nullptr)
        {
            result = g_pVulkanDriver->m_Swapchain.Initialize(
                createInfo,
                g_pVulkanDriver->m_Gpu,
                g_pVulkanDriver->m_Surface);

            if (SAVANNA_GFX_SUCCESS(result) && pOutSwapchainHandle != nullptr)
            {
                *pOutSwapchainHandle = (seGfxHandle)&g_pVulkanDriver->m_Swapchain;
            }
        }

        return result;
    }

    seGfxErrorCode VkDriver::CreateShaderModule(
        const seGfxShaderCreateInfo& createInfo,
        seGfxShader& outShaderModuleHandle)
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

    seJobHandle VkDriver::CreateShaderModulesAsync(
        const seGfxShaderCreateInfo* pCreateInfos,
        const size_t createInfoCount,
        seGfxShader** const ppOutShaderModuleHandles)
    {
        JobHandle jobHandle = k_InvalidJobHandle;
        if (g_pVulkanDriver != nullptr)
        {
            jobHandle = g_pVulkanDriver->m_ShaderModuleCache.CreateShaderModulesAsync(
                g_pVulkanDriver->m_Gpu,
                const_cast<seGfxShaderCreateInfo*>(pCreateInfos),
                createInfoCount,
                ppOutShaderModuleHandles);
        }

        return jobHandle;
    }

    GfxErrorCode VkDriver::CreateInstance(
        const seGfxDriverCreateInfo& createInfo,
        seVkDriverCreateInfo& driverCreateInfo)
    {
        VkApplicationInfo appInfo = utils::k_SavannaDefaultVulkanAppInfo;
        appInfo.pApplicationName = createInfo.m_pApplicationName == nullptr
            ? "Savanna"
            : createInfo.m_pApplicationName;

        seVkInstanceCreateArgs& instanceCreateArgs = driverCreateInfo.m_InstanceCreateArgs;
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

        // Load create instance, get proc addr, and enumerate extensions
        LoadLibraryPhase0();

        utils::PopulateInstanceExtensions(enabledInstanceExtensions,
            instanceCreateArgs.m_ppEnabledInstanceExtensions,
            instanceCreateArgs.m_EnabledInstanceExtensionCount);

        for (uint32 i = 0; i < instanceCreateArgs.m_EnabledLayerCount; ++i)
        {
            enabledInstanceLayers.push_back(instanceCreateArgs.m_ppEnabledLayers[i]);
        }

        VkInstanceCreateInfo instanceCreateInfo {};
        utils::PopulateInstanceCreateInfo(instanceCreateInfo,
            enabledInstanceExtensions.data(), enabledInstanceExtensions.size(),
            enabledInstanceLayers.data(), enabledInstanceLayers.size());
        instanceCreateInfo.pApplicationInfo = &appInfo;

        VkResult result = vkCreateInstance(&instanceCreateInfo, VkAllocator::Get(), &m_Instance);

        if (VK_SUCCEEDED(result) && driverCreateInfo.m_EnableValidationLayers)
        {
            utils::DebugMessenger::Initialize(m_Instance, VkAllocator::Get());
        }

        return ResultToErrorCode(result);
    }

    void VkDriver::PopulateDriverInterface(
        seGfxDriverInterface &outDriverInterface) {
#define FILL_OUT_INTERFACE_FUNC(__func) .m_pfn##__func = VkDriver::__func

        constexpr seGfxDriverInterface k_VulkanDriverInterface
        {
            FILL_OUT_INTERFACE_FUNC(Initialize),
            FILL_OUT_INTERFACE_FUNC(Destroy),
            FILL_OUT_INTERFACE_FUNC(GetDriver),
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
        utils::DebugMessenger::Destroy();

        if (m_Instance != VK_NULL_HANDLE)
        {
            vkDestroyInstance(m_Instance, VkAllocator::Get());
            m_Instance = VK_NULL_HANDLE;
        }
    }

} // namespace savanna::Gfx::vk
