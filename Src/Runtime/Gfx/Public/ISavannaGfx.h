/**
 * @file SavannaGfx.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
 * @version 0.1
 * @date 2023-09-19
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef I_SAVANNA_GFX_H
#define I_SAVANNA_GFX_H
// TODO @DavidMohrhardt: Need to update all the structs defined here to have type field so you can cast to the correct type

#include "Public/ISavannaEngine.h"
#include "ISavannaGfxFormat.h"
#include "Memory/Public/ISavannaMemory.h"

// Potentially external headers
#ifndef I_SAVANNA_CONCURRENCY_H
    typedef se_intptr seJobHandle;
#endif

#undef SAVANNA_GFX_SUCCESS
#define SAVANNA_GFX_SUCCESS(__operationOrValue) \
    ((__operationOrValue) == kSavannaGfxErrorCodeSuccess)

#undef SAVANNA_GFX_FAILURE
#define SAVANNA_GFX_FAILURE(__operationOrValue) \
    ((__operationOrValue) >= kSavannaGfxErrorCodeFailure)

#undef SAVANNA_GFX_FATAL_ERROR
#define SAVANNA_GFX_FATAL_ERROR(__operationOrValue) \
    ((__operationOrValue) >= kSavannaGfxErrorCodeFatalError)

/**
 * @brief A list of error codes for the graphics system.
 *
 */
typedef enum seGfxErrorCode : se_uint32
{
    /**
     * @brief Operation was successful.
     */
    kSavannaGfxErrorCodeSuccess = 0,

    /**
     * @brief Operation was a failure.
     * @note This error code is not intended to be used for specific errors
     *       but rather as a value to compare against to check if an operation
     *       was successful or not.
     */
    kSavannaGfxErrorCodeFailure,

    /**
     * @brief The function is not implemented.
     */
    kSavannaGfxErrorCodeNotImplemented,

    /**
     * @brief The graphics system has already been initialized.
     */
    kSavannaGfxErrorCodeContextAlreadyInitialized,

    /**
     * @brief The operation was an unrecoverable error.
     * @note This error code is not intended to be used for specific errors
     *       but rather as a value to compare against to check if an operation
     *       was successful or not. Some implementations may consider an unimplemented
     *       function to be a fatal error or not.
     *
     */
    kSavannaGfxErrorCodeFatalError,

    /**
     * @brief There was an unknown error.
     */
    kSavannaGfxErrorCodeUnknownError,

    /**
     * @brief There was not enough memory to complete the operation.
    */
    kSavannaGfxErrorCodeOutOfMemory,

    /**
     * @brief There was not enough memory on the device to
     * complete the operation.
    */
    kSavannaGfxErrorCodeOutOfDeviceMemory,

    /**
     * @brief The allocator interface provided was invalid.
     *        To avoid this error, acquire the default allocator interface
     *       using @SavannaMemoryManagerTryGetAllocatorInterfaceForAllocatorKind.
     */
    kSavannaGfxErrorCodeInvalidAllocatorInterface,

    /**
     * @brief The function was provided an invalid argument.
     */
    kSavannaGfxErrorCodeInvalidArgument,

    /**
     * @brief The graphics system or object has not been initialized.
     */
    kSavannaGfxErrorCodeNotInitialized,

    /**
     * @brief The graphics was not able to create a IGfxDriver.
     */
    kSavannaGfxErrorCodeUnsupportedGfxBackend,

    /**
     * @brief The graphics driver was unable to create a graphics driver.
     */
    kSavannaGfxErrorCodeUnableToCreateGfxDriver,

    /**
     * @brief The driver interface provided was invalid.
     *
     */
    kSavannaGfxErrorCodeInvalidDriverInterface,

    /**
     * @brief The graphics driver has already been created.
     * @note This error code may not always be considered an error.
     */
    kSavannaGfxErrorCodeGfxDriverAlreadyCreated,

    /**
     * @brief Indicates a platform specific error.
     *
     */
    kSavannaGfxErrorCodePlatformSpecificError,
} seGfxErrorCode;

/**
 * @brief The type of graphics backend.
 *
 */
typedef enum seGfxBackend : se_uint32
{
    kSavannaGfxApiNone = 0,
    kSavannaGfxApiVulkan,

    // Unsupported currently
    // kSavannaGfxApiDirectX12,
    // kSavannaGfxApiDirectX11,
    // kSavannaGfxApiOpenGL,
    // kSavannaGfxApiOpenGLES,
    // kSavannaGfxApiMetal,

    // This is for unknown external backends
    kSavannaGfxExternalBackend,

    kSavannaGfxApiCount,
} seGfxBackend;

/**
 * @brief The supported graphics backends.
 *
 */
typedef enum seGfxSupportedBackend : se_uint8
{
    kSavannaSupportedGfxApiNone = kSavannaGfxApiNone,
    kSavannaSupportedGfxApiAll = (se_uint8)(~0),

    kSavannaSupportedGfxApiVulkan = 1 << kSavannaGfxApiVulkan,
    // kSavannaSupportedGfxApiDirectX12 = 1 << kSavannaGfxApiDirectX12,
    // kSavannaSupportedGfxApiDirectX11 = 1 << kSavannaGfxApiDirectX11,
    // kSavannaSupportedGfxApiOpenGL = 1 << kSavannaGfxApiOpenGL,
    // kSavannaSupportedGfxApiOpenGLES = 1 << kSavannaGfxApiOpenGLES,
    // kSavannaSupportedGfxApiMetal = 1 << kSavannaGfxApiMetal,
} seGfxSupportedBackend;

/**
 * @brief A handle to a graphics object.
 *
 */
typedef intptr_t seGfxHandle;

/**
 * @brief Defines the invalid handle for a graphics object.
 *
 */
#define SE_GFX_INVALID_HANDLE (seGfxHandle)(0)

/**
 * @brief A helper macro for typedefing the graphics handles.
 *
 */
#define SE_GFX_HANDLE_TYPEDEF(__handleType) \
    typedef seGfxHandle se##__handleType##Handle;

/**
 * @brief A handle to a graphics context. There can only be zero or one graphics context.
 *
 */
SE_GFX_HANDLE_TYPEDEF(GfxContext);

/**
 * @brief A handle to a graphics driver. There can only be zero or one graphics driver.
 *
 */
SE_GFX_HANDLE_TYPEDEF(GfxDriver);

/**
 * @brief A handle to a native command buffer. This is a handle to the native graphics API.
 *
 */
// SE_GFX_HANDLE_TYPEDEF(GfxCommandBuffer);
// SE_GFX_HANDLE_TYPEDEF(GfxFence);
// SE_GFX_HANDLE_TYPEDEF(GfxSemaphore);
SE_GFX_HANDLE_TYPEDEF(GfxBuffer);
// SE_GFX_HANDLE_TYPEDEF(GfxImage);
// SE_GFX_HANDLE_TYPEDEF(GfxImageView);
// SE_GFX_HANDLE_TYPEDEF(GfxSampler);
SE_GFX_HANDLE_TYPEDEF(GfxShader);
// SE_GFX_HANDLE_TYPEDEF(GfxPipelineLayout);
// SE_GFX_HANDLE_TYPEDEF(GfxRenderPass);
// SE_GFX_HANDLE_TYPEDEF(GfxPipeline);
// SE_GFX_HANDLE_TYPEDEF(GfxDescriptorSetLayout);
// SE_GFX_HANDLE_TYPEDEF(GfxDescriptorPool);
// SE_GFX_HANDLE_TYPEDEF(GfxDescriptorSet);
// SE_GFX_HANDLE_TYPEDEF(GfxFramebuffer);

#undef SE_GFX_HANDLE_TYPEDEF

typedef struct seGfxContextCreateInfo
{
    const char* m_pApplicationName;

    const seAllocatorInterface* m_pAllocatorInterface;

    void* m_pUserData;
} seGfxContextCreateInfo;

/// Graphics Driver Section
typedef struct seGfxDriverCreateInfo
{
    /**
     * @brief The name of the application.
     *
     */
    const char* m_pApplicationName;

    /**
     * @brief The type of backend to create.
     */
    seGfxBackend m_RequestedBackendType;

    /**
     * @brief The allocator the driver should use.
     */
    const seAllocatorInterface* m_pAllocationInterface;

    /**
     * @brief A pointer to potential extension structs to modify
     * the creation of the driver. This is used to allow for
     * future extensions to the driver creation.
     */
    void* m_pNext;

    /**
     * @brief User provided additional data. Used to allow users
     * to pass additional information for their own driver implementations.
     *
     * Savanna will never overwrite this field.
     */
    void* m_pUserData;
} seGfxDriverCreateInfo;

/**
 * @brief A list of create infos for the graphics drivers.
 * This is used to allow for multiple drivers to be created.
 *
 */
typedef struct seGfxDriverCreateInfoList
{
    /**
     * @brief A pointer to the list of create infos.
     */
    seGfxDriverCreateInfo* m_pDriverCreateInfos;

    /**
     * @brief The number of create infos in the list.
     */
    se_uint32 m_CreateInfoCount;

    /**
     * @brief User provided additional data. Savanna does not use this but
     * it allows for the user to pass in additional data to the driver.
     */
    void* m_pUserData;
} seGfxDriverCreateInfoList;

// Swapchain Section
typedef struct seGfxSwapchainCreateInfo
{
    /**
     * @brief The width of the swapchain.
     */
    se_uint32 m_Width;

    /**
     * @brief The height of the swapchain.
     */
    se_uint32 m_Height;

    /**
     * @brief The number of images in the swapchain.
     */
    se_uint32 m_ImageCount;

    /**
     * @brief The format of the swapchain images.
     */
    seGfxTextureFormat m_Format;
} seGfxSwapchainCreateInfo;

/**
 * @brief A function pointer defining the function signature for creating a graphics driver.
 */
typedef seGfxErrorCode (*sepfnGfxDriverCreate)(const seGfxDriverCreateInfo& pCreateInfo);

/**
 * @brief A function pointer defining the function signature for destroying a graphics driver.
 */
typedef seGfxErrorCode (*sepfnGfxDriverDestroy)();

/**
 * @brief A function pointer defining the function signature for retrieving the real, underlying driver handle.
 */
typedef seGfxDriverHandle (*sepfnGfxDriverGetDriverHandle)();

/**
 * @brief A function pointer defining the function signature for requesting a swapchain.
 */
typedef seGfxErrorCode (*sepfnGfxDriverCreateSwapchain)(const seGfxSwapchainCreateInfo& createInfo, seGfxHandle* const pOutSwapchainHandle);


/// Shaders Section

#define k_SavannaGfxInvalidShaderModuleHandle (0x0)

/**
 * @brief The stage of the shader module.
 *
 */
typedef enum seGfxShaderStage : se_uint32
{
    kSavannaGfxShaderStageVertex = 0,
    kSavannaGfxShaderStageFragment,
    kSavannaGfxShaderStageCompute,
    kSavannaGfxShaderStageGeometry,
    kSavannaGfxShaderStageTessellationControl,
    kSavannaGfxShaderStageTessellationEvaluation,
    kSavannaGfxShaderStageRayGeneration,
    kSavannaGfxShaderStageIntersection,
    kSavannaGfxShaderStageAnyHit,
    kSavannaGfxShaderStageClosestHit,
    kSavannaGfxShaderStageMiss,
    kSavannaGfxShaderStageCallable,
    kSavannaGfxShaderStageTask,
    kSavannaGfxShaderStageMesh,
    kSavannaGfxShaderStageCount,
} seGfxShaderStage;

/**
 * @brief The type of shader module.
 *
 */
typedef enum seGfxShaderType : se_uint32
{
    kSavannaGfxShaderTypeUnknown = 0,
    kSavannaGfxShaderTypeSpirv,

    // Currently unsupported
    // kSavannaGfxShaderTypeGlsl,
    // kSavannaGfxShaderTypeHlsl,

    kSavannaGfxShaderTypeCount,
} seGfxShaderType;

/**
 * @brief The create info for a shader module.
 *
 */
typedef struct seGfxShaderCreateInfo
{
    /**
     * @brief The type of shader module.
     *
     */
    seGfxShaderType m_Type;

    /**
     * @brief The stage of the shader module.
     *
     */
    seGfxShaderStage m_Stage;

    /**
     * @brief The size of the shader module data.
     *
     */
    se_uint32 m_Size;

    /**
     * @brief The shader module data.
     *
     */
    const void* m_pData;

    /**
     * @brief The allocator interface to use for the shader module.
     *
     */
    const seAllocatorInterface* m_pAllocatorInterface;

    /**
     * @brief The user data to pass to the shader module.
     *
     */
    void* m_pUserData;
} seGfxShaderCreateInfo;

/**
 * @brief A list of shader module create infos.
 *
 */
typedef struct seGfxShaderCreateInfoList
{
    /**
     * @brief The list of shader module create infos.
     *
     */
    seGfxShaderCreateInfo* m_pCreateInfos;

    /**
     * @brief The number of shader module create infos.
     *
     */
    se_uint32 m_CreateInfoCount;

    /**
     * @brief The allocator interface to use for the shader module.
     *
     */
    const seAllocatorInterface* m_pAllocatorInterface;

    /**
     * @brief The user data to pass to the shader module.
     *
     */
    void* m_pUserData;
} seGfxShaderCreateInfoList;

/**
 * @brief A function pointer defining the function signature for requesting shader module creation.
 */
typedef seGfxErrorCode (*sepfnGfxDriverCreateShaderModule)(
    const seGfxShaderCreateInfo& createInfo,
    seGfxShaderHandle& outShaderModuleHandle);

/**
 * @brief A function pointer defining the function signature for requesting shader module creation
 * for a given number of shader module create infos. This function is asynchronous.
 *
 * @note This function is not required to be implemented and can return
 * kSavannaGfxErrorCodeNotImplemented or be nullptr.
 */
typedef seJobHandle (*sepfnGfxDriverCreateShaderModulesAsync)(
    const seGfxShaderCreateInfo* pCreateInfos,
    const size_t createInfoCount,
    seGfxShaderHandle** const ppOutShaderModuleHandles);

/// Pipelines

/**
 * @brief The type of pipeline.
 *
 */
typedef enum seGfxPipelineType : se_uint32
{
    kSavannaGfxPipelineTypeGraphics = 0,
    kSavannaGfxPipelineTypeCompute,
    kSavannaGfxPipelineTypeRayTracing,
    kSavannaGfxPipelineTypeCount,
} seGfxPipelineType;

/**
 * @brief The flags for the graphics shader stages.
 *
 */
typedef enum seGfxGraphicsShaderStageFlags : se_uint32
{
    /**
     * @brief Indicates that no shader stages are enabled.
     *
    */
    kSavannaGfxShaderStageNone = 0,

    /**
     * @brief Indicates that the vertex shader stage is enabled.
     *
     * @note This is the default value and must be explicitly true for a valid pipeline.
     */
    kSavannaGfxVertexShaderStage = 1 << kSavannaGfxShaderStageVertex,

    /**
     * @brief Indicates that the fragment shader stage is enabled.
     *
     * @note This is a default value and must be explicitly true for a valid pipeline.
     */
    kSavannaGfxFragmentShaderStage = 1 << kSavannaGfxShaderStageFragment,

    /**
     * @brief Indicates that the minimal graphics shader stages are enabled.
     *
     */
    kSavannaGfxMinimalGraphicsShaderStage = kSavannaGfxVertexShaderStage | kSavannaGfxFragmentShaderStage,

    kSavannaGfxGeometryShaderStage = 1 << kSavannaGfxShaderStageGeometry,
    kSavannaGfxTessellationControlShaderStage = 1 << kSavannaGfxShaderStageTessellationControl,
    kSavannaGfxTessellationEvaluationShaderStage = 1 << kSavannaGfxShaderStageTessellationEvaluation,
} seGfxGraphicsShaderStageFlags;

typedef struct seGfxShaderStageCreateInfo
{
    seGfxShaderHandle m_ShaderModule;
    const char* m_pEntryPoint;
    seGfxShaderStage m_Stage;
    void* pNext;
} seGfxShaderStageCreateInfo;

typedef struct seGfxGraphicsPipelineCreateInfo
{
    /**
     * @brief A linked list of shader stages. Must have at least a vertex and fragment shader.
     *
     * @decl_spec_rule Must have at least a vertex and fragment shader.
     */
    seGfxShaderStageCreateInfo* m_pShaderStages;

    // Vertex input state
    // Input assembly state
    // Tessellation state

    // Fixed function pipeline inputs
    // seGfxGraphicsViewportStateCreateInfo m_ViewportState;
    // seGfxGraphicsRasterizationStateCreateInfo m_RasterizationState;
    // seGfxGraphicsMultisampleStateCreateInfo m_MultisampleState;
    // seGfxGraphicsDepthStencilStateCreateInfo m_DepthStencilState;
    // seGfxGraphicsColorBlendStateCreateInfo m_ColorBlendState;
    // seGfxGraphicsDynamicStateCreateInfo m_DynamicState;

    void* pNext;
} seGfxGraphicsPipelineCreateInfo;

typedef struct seGfxComputePipelineCreateInfo
{
    // Required
    seGfxShaderHandle m_ComputeShader;

    void* pNext;
} seGfxComputePipelineCreateInfo;

typedef struct seGfxRayTracingPipelineCreateInfo
{
    // Required
    seGfxShaderHandle m_RayGenerationShader;

    void* pNext;
} seGfxRayTracingPipelineCreateInfo;

/**
 * @brief Retrieves the type of backend the driver represents.
 *
 */
typedef seGfxBackend (*sepfnGfxDriverGetBackend)();

/**
 * @brief a struct containing the function pointers for the graphics driver interface.
 * This is used to implement an external graphics driver interface for Savanna to use.
 *
 */
typedef struct seGfxDriverInterface
{
    /**
     * @brief A pointer to the function for creating a graphics driver.
     */
    sepfnGfxDriverCreate m_pfnInitialize;

    /**
     * @brief A pointer to the function for destroying a graphics driver.
     */
    sepfnGfxDriverDestroy m_pfnDestroy;

    /**
     * @brief A pointer to the function for retrieving the real, underlying driver handle.
     */
    sepfnGfxDriverGetDriverHandle m_pfnGetDriverHandle;

    /**
     * @brief A pointer to the function for requesting a initialization of a swapchain.
     *
     */
    sepfnGfxDriverCreateSwapchain m_pfnCreateSwapchain;

    /**
     * @brief A pointer to the function for requesting a shader module.
     *
     */
    sepfnGfxDriverCreateShaderModule m_pfnCreateShaderModule;

    /**
     * @brief A pointer to the function for requesting multiple shader modules be created
     * asynchronously.
     *
     * @note This function is not required to be implemented and can be nullptr.
     *
     */
    sepfnGfxDriverCreateShaderModulesAsync m_pfnCreateShaderModulesAsync;

    /**
     * @brief TODO @David.Mohrhardt Document
     *
     */
    sepfnGfxDriverGetBackend m_pfnGetBackend;

    /**
     * @brief TODO @David.Mohrhardt Document
     */
    void* m_pUserData;
} seGfxDriverInterface;

/**
 * @brief Provides the set of capabilities for the graphics system.
 *
 */
typedef struct seGfxCapabilities
{
    seGfxSupportedBackend m_SupportedGfxApi;
    seGfxBackend m_ActiveGfxApi;

    void* m_pPlatformCapabilities;
} seGfxCapabilities;

/// API Section

/**
 * @brief Initializes the graphics context. This must be called before any other
 *       graphics functions are called.
 * @param pCreateInfo The create info for the graphics context.
 * @return seGfxErrorCode The error code for the operation.
 * @note This function is not thread safe.
 */
SAVANNA_EXPORT(seGfxErrorCode) SavannaGfxInit(const seGfxContextCreateInfo* const pCreateInfo);

/**
 * @brief Shuts the graphics context down. This must be called to ensure disposal
 *       of all graphics resources.
 *
 * @return seGfxErrorCode The error code for the operation.
 * @note This function is not thread safe and must be called on the same thread that
 *      called SavannaGfxInit.
 */
SAVANNA_EXPORT(seGfxErrorCode) SavannaGfxShutdown();

/**
 * @brief Creates a graphics driver for the specified backend.
 * @param pCreateInfo The list of create infos for the graphics drivers.
 * The list will be iterated over until a driver is successfully created.
 * @return seGfxErrorCode The error code for the operation.
 *
 * @note This function is not thread safe and must be called on
 *      the same thread that called SavannaGfxInit.
 */
SAVANNA_EXPORT(seGfxErrorCode) SavannaGfxContextCreateDriver(
    const seGfxDriverCreateInfoList* const pCreateInfoList);

/**
 * @brief Acquires the supported graphics backends for the current platform.
 * @return seGfxSupportedBackend The supported graphics backends.
 *
 * @note This function can be called prior to SavannaGfxInit.
 */
SAVANNA_EXPORT(seGfxSupportedBackend) SavannaGfxGetSupportedGraphicsBackends();

/**
 * @brief Gets the active graphics backend.
 * @return seGfxBackend The active graphics backend.
 */
SAVANNA_EXPORT(seGfxBackend) SavannaGfxGetActiveGraphicsBackend();

/**
 * @brief TODO
 *
 */
SAVANNA_EXPORT(seGfxHandle) SavannaGfxGetDriverHandle();

/**
 * @brief TODO
 *
 */
SAVANNA_EXPORT(seGfxErrorCode) SavannaGfxCreateSwapchain(
    const seGfxSwapchainCreateInfo* const pCreateInfo,
    seGfxHandle* const pOutSwapchainHandle);

SAVANNA_EXPORT(seGfxErrorCode) SavannaGfxCreateShaderModule(
    const seGfxShaderCreateInfo& createInfo,
    seGfxShaderHandle& outShaderModuleHandle);

SAVANNA_EXPORT(seJobHandle) SavannaGfxCreateShaderModulesAsync(
    const seGfxShaderCreateInfo* pCreateInfos,
    const size_t createInfoCount,
    seGfxShaderHandle** const ppOutShaderModuleHandles);

/**
 * @brief Gets the capabilities of the graphics system. Is valid even before
 *      SavannaGfxInit has been called though the returned capabilities may
 *      not be completely filled out until after SavannaGfxContextCreateDriver
 *      has been called.
 */
SAVANNA_EXPORT(const seGfxCapabilities) SavannaGfxGetCapabilities();

#endif // !I_SAVANNA_GFX_H
