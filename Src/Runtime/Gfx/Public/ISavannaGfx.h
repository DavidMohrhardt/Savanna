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
    typedef se_intptr se_JobHandle_t;
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
typedef enum se_GfxErrorCode_t : se_uint32
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
} se_GfxErrorCode_t;

/**
 * @brief The type of graphics backend.
 *
 */
typedef enum se_GfxBackend_t : se_uint32
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
} se_GfxBackend_t;

/**
 * @brief The supported graphics backends.
 *
 */
typedef enum se_GfxSupportedBackend_t : se_uint8
{
    kSavannaSupportedGfxApiNone = kSavannaGfxApiNone,
    kSavannaSupportedGfxApiAll = (se_uint8)(~0),

    kSavannaSupportedGfxApiVulkan = 1 << kSavannaGfxApiVulkan,
    // kSavannaSupportedGfxApiDirectX12 = 1 << kSavannaGfxApiDirectX12,
    // kSavannaSupportedGfxApiDirectX11 = 1 << kSavannaGfxApiDirectX11,
    // kSavannaSupportedGfxApiOpenGL = 1 << kSavannaGfxApiOpenGL,
    // kSavannaSupportedGfxApiOpenGLES = 1 << kSavannaGfxApiOpenGLES,
    // kSavannaSupportedGfxApiMetal = 1 << kSavannaGfxApiMetal,
} se_GfxSupportedBackend_t;

/**
 * @brief A handle to a graphics object.
 *
 */
typedef intptr_t se_GfxHandle_t;

/**
 * @brief Defines the invalid handle for a graphics object.
 *
 */
#define SE_GFX_INVALID_HANDLE (se_GfxHandle_t)(0)

/**
 * @brief A helper macro for typedefing the graphics handles.
 *
 */
#define SE_GFX_HANDLE_TYPEDEF(__handleType) \
    typedef se_GfxHandle_t se_##__handleType##Handle_t;

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

typedef struct se_GfxContextCreateInfo_t
{
    const char* m_pApplicationName;

    const se_AllocatorInterface_t* m_pAllocatorInterface;

    void* m_pUserData;
} se_GfxContextCreateInfo_t;

/// Graphics Driver Section
typedef struct se_GfxDriverCreateInfo_t
{
    /**
     * @brief The name of the application.
     *
     */
    const char* m_pApplicationName;

    /**
     * @brief The type of backend to create.
     */
    se_GfxBackend_t m_RequestedBackendType;

    /**
     * @brief The allocator the driver should use.
     */
    const se_AllocatorInterface_t* m_pAllocationInterface;

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
} se_GfxDriverCreateInfo_t;

/**
 * @brief A list of create infos for the graphics drivers.
 * This is used to allow for multiple drivers to be created.
 *
 */
typedef struct se_GfxDriverCreateInfoList_t
{
    /**
     * @brief A pointer to the list of create infos.
     */
    se_GfxDriverCreateInfo_t* m_pDriverCreateInfos;

    /**
     * @brief The number of create infos in the list.
     */
    se_uint32 m_CreateInfoCount;

    /**
     * @brief User provided additional data. Savanna does not use this but
     * it allows for the user to pass in additional data to the driver.
     */
    void* m_pUserData;
} se_GfxDriverCreateInfoList_t;

// Swapchain Section
typedef struct se_GfxSwapchainCreateInfo_t
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
    se_GfxTextureFormat_t m_Format;
} se_GfxSwapchainCreateInfo_t;

/**
 * @brief A function pointer defining the function signature for creating a graphics driver.
 */
typedef se_GfxErrorCode_t (*se_pfnGfxDriverCreate_t)(const se_GfxDriverCreateInfo_t& pCreateInfo);

/**
 * @brief A function pointer defining the function signature for destroying a graphics driver.
 */
typedef se_GfxErrorCode_t (*se_pfnGfxDriverDestroy_t)();

/**
 * @brief A function pointer defining the function signature for retrieving the real, underlying driver handle.
 */
typedef se_GfxDriverHandle_t (*se_pfnGfxDriverGetDriverHandle_t)();

/**
 * @brief A function pointer defining the function signature for requesting a swapchain.
 */
typedef se_GfxErrorCode_t (*se_pfnGfxDriverCreateSwapchain_t)(const se_GfxSwapchainCreateInfo_t& createInfo, se_GfxHandle_t* const pOutSwapchainHandle);


/// Shaders Section

#define k_SavannaGfxInvalidShaderModuleHandle (0x0)

/**
 * @brief The stage of the shader module.
 *
 */
typedef enum se_GfxShaderStage_t : se_uint32
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
} se_GfxShaderStage_t;

/**
 * @brief The type of shader module.
 *
 */
typedef enum se_GfxShaderType_t : se_uint32
{
    kSavannaGfxShaderTypeUnknown = 0,
    kSavannaGfxShaderTypeSpirv,

    // Currently unsupported
    // kSavannaGfxShaderTypeGlsl,
    // kSavannaGfxShaderTypeHlsl,

    kSavannaGfxShaderTypeCount,
} se_GfxShaderType_t;

/**
 * @brief The create info for a shader module.
 *
 */
typedef struct se_GfxShaderCreateInfo_t
{
    /**
     * @brief The type of shader module.
     *
     */
    se_GfxShaderType_t m_Type;

    /**
     * @brief The stage of the shader module.
     *
     */
    se_GfxShaderStage_t m_Stage;

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
    const se_AllocatorInterface_t* m_pAllocatorInterface;

    /**
     * @brief The user data to pass to the shader module.
     *
     */
    void* m_pUserData;
} se_GfxShaderCreateInfo_t;

/**
 * @brief A list of shader module create infos.
 *
 */
typedef struct se_GfxShaderCreateInfoList_t
{
    /**
     * @brief The list of shader module create infos.
     *
     */
    se_GfxShaderCreateInfo_t* m_pCreateInfos;

    /**
     * @brief The number of shader module create infos.
     *
     */
    se_uint32 m_CreateInfoCount;

    /**
     * @brief The allocator interface to use for the shader module.
     *
     */
    const se_AllocatorInterface_t* m_pAllocatorInterface;

    /**
     * @brief The user data to pass to the shader module.
     *
     */
    void* m_pUserData;
} se_GfxShaderCreateInfoList_t;

/**
 * @brief A function pointer defining the function signature for requesting shader module creation.
 */
typedef se_GfxErrorCode_t (*se_pfnGfxDriverCreateShaderModule_t)(
    const se_GfxShaderCreateInfo_t& createInfo,
    se_GfxShaderHandle_t& outShaderModuleHandle);

/**
 * @brief A function pointer defining the function signature for requesting shader module creation
 * for a given number of shader module create infos. This function is asynchronous.
 *
 * @note This function is not required to be implemented and can return
 * kSavannaGfxErrorCodeNotImplemented or be nullptr.
 */
typedef se_JobHandle_t (*se_pfnGfxDriverCreateShaderModulesAsync_t)(
    const se_GfxShaderCreateInfo_t* pCreateInfos,
    const size_t createInfoCount,
    se_GfxShaderHandle_t** const ppOutShaderModuleHandles);

/// Pipelines

/**
 * @brief The type of pipeline.
 *
 */
typedef enum se_GfxPipelineType_t : se_uint32
{
    kSavannaGfxPipelineTypeGraphics = 0,
    kSavannaGfxPipelineTypeCompute,
    kSavannaGfxPipelineTypeRayTracing,
    kSavannaGfxPipelineTypeCount,
} se_GfxPipelineType_t;

/**
 * @brief The flags for the graphics shader stages.
 *
 */
typedef enum se_GfxGraphicsShaderStageFlags_t : se_uint32
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
} se_GfxGraphicsShaderStageFlags_t;

typedef struct se_GfxShaderStageCreateInfo_t
{
    se_GfxShaderHandle_t m_ShaderModule;
    const char* m_pEntryPoint;
    se_GfxShaderStage_t m_Stage;
    void* pNext;
} se_GfxShaderStageCreateInfo_t;

typedef struct se_GfxGraphicsPipelineCreateInfo_t
{
    /**
     * @brief A linked list of shader stages. Must have at least a vertex and fragment shader.
     *
     * @decl_spec_rule Must have at least a vertex and fragment shader.
     */
    se_GfxShaderStageCreateInfo_t* m_pShaderStages;

    // Vertex input state
    // Input assembly state
    // Tessellation state

    // Fixed function pipeline inputs
    // se_GfxGraphicsViewportStateCreateInfo_t m_ViewportState;
    // se_GfxGraphicsRasterizationStateCreateInfo_t m_RasterizationState;
    // se_GfxGraphicsMultisampleStateCreateInfo_t m_MultisampleState;
    // se_GfxGraphicsDepthStencilStateCreateInfo_t m_DepthStencilState;
    // se_GfxGraphicsColorBlendStateCreateInfo_t m_ColorBlendState;
    // se_GfxGraphicsDynamicStateCreateInfo_t m_DynamicState;

    void* pNext;
} se_GfxGraphicsPipelineCreateInfo_t;

typedef struct se_GfxComputePipelineCreateInfo_t
{
    // Required
    se_GfxShaderHandle_t m_ComputeShader;

    void* pNext;
} se_GfxComputePipelineCreateInfo_t;

typedef struct se_GfxRayTracingPipelineCreateInfo_t
{
    // Required
    se_GfxShaderHandle_t m_RayGenerationShader;

    void* pNext;
} se_GfxRayTracingPipelineCreateInfo_t;

/**
 * @brief Retrieves the type of backend the driver represents.
 *
 */
typedef se_GfxBackend_t (*se_pfnGfxDriverGetBackend_t)();

/**
 * @brief a struct containing the function pointers for the graphics driver interface.
 * This is used to implement an external graphics driver interface for Savanna to use.
 *
 */
typedef struct se_GfxDriverInterface_t
{
    /**
     * @brief A pointer to the function for creating a graphics driver.
     */
    se_pfnGfxDriverCreate_t m_pfnInitialize;

    /**
     * @brief A pointer to the function for destroying a graphics driver.
     */
    se_pfnGfxDriverDestroy_t m_pfnDestroy;

    /**
     * @brief A pointer to the function for retrieving the real, underlying driver handle.
     */
    se_pfnGfxDriverGetDriverHandle_t m_pfnGetDriverHandle;

    /**
     * @brief A pointer to the function for requesting a initialization of a swapchain.
     *
     */
    se_pfnGfxDriverCreateSwapchain_t m_pfnCreateSwapchain;

    /**
     * @brief A pointer to the function for requesting a shader module.
     *
     */
    se_pfnGfxDriverCreateShaderModule_t m_pfnCreateShaderModule;

    /**
     * @brief A pointer to the function for requesting multiple shader modules be created
     * asynchronously.
     *
     * @note This function is not required to be implemented and can be nullptr.
     *
     */
    se_pfnGfxDriverCreateShaderModulesAsync_t m_pfnCreateShaderModulesAsync;

    /**
     * @brief TODO @David.Mohrhardt Document
     *
     */
    se_pfnGfxDriverGetBackend_t m_pfnGetBackend;

    /**
     * @brief TODO @David.Mohrhardt Document
     */
    void* m_pUserData;
} se_GfxDriverInterface_t;

/**
 * @brief Provides the set of capabilities for the graphics system.
 *
 */
typedef struct se_GfxCapabilities_t
{
    se_GfxSupportedBackend_t m_SupportedGfxApi;
    se_GfxBackend_t m_ActiveGfxApi;

    void* m_pPlatformCapabilities;
} se_GfxCapabilities_t;

/// API Section

/**
 * @brief Initializes the graphics context. This must be called before any other
 *       graphics functions are called.
 * @param pCreateInfo The create info for the graphics context.
 * @return se_GfxErrorCode_t The error code for the operation.
 * @note This function is not thread safe.
 */
SAVANNA_EXPORT(se_GfxErrorCode_t) SavannaGfxInit(const se_GfxContextCreateInfo_t* const pCreateInfo);

/**
 * @brief Shuts the graphics context down. This must be called to ensure disposal
 *       of all graphics resources.
 *
 * @return se_GfxErrorCode_t The error code for the operation.
 * @note This function is not thread safe and must be called on the same thread that
 *      called SavannaGfxInit.
 */
SAVANNA_EXPORT(se_GfxErrorCode_t) SavannaGfxShutdown();

/**
 * @brief Creates a graphics driver for the specified backend.
 * @param pCreateInfo The list of create infos for the graphics drivers.
 * The list will be iterated over until a driver is successfully created.
 * @return se_GfxErrorCode_t The error code for the operation.
 *
 * @note This function is not thread safe and must be called on
 *      the same thread that called SavannaGfxInit.
 */
SAVANNA_EXPORT(se_GfxErrorCode_t) SavannaGfxContextCreateDriver(
    const se_GfxDriverCreateInfoList_t* const pCreateInfoList);

/**
 * @brief Acquires the supported graphics backends for the current platform.
 * @return se_GfxSupportedBackend_t The supported graphics backends.
 *
 * @note This function can be called prior to SavannaGfxInit.
 */
SAVANNA_EXPORT(se_GfxSupportedBackend_t) SavannaGfxGetSupportedGraphicsBackends();

/**
 * @brief Gets the active graphics backend.
 * @return se_GfxBackend_t The active graphics backend.
 */
SAVANNA_EXPORT(se_GfxBackend_t) SavannaGfxGetActiveGraphicsBackend();

/**
 * @brief TODO
 *
 */
SAVANNA_EXPORT(se_GfxHandle_t) SavannaGfxGetDriverHandle();

/**
 * @brief TODO
 *
 */
SAVANNA_EXPORT(se_GfxErrorCode_t) SavannaGfxCreateSwapchain(
    const se_GfxSwapchainCreateInfo_t* const pCreateInfo,
    se_GfxHandle_t* const pOutSwapchainHandle);

SAVANNA_EXPORT(se_GfxErrorCode_t) SavannaGfxCreateShaderModule(
    const se_GfxShaderCreateInfo_t& createInfo,
    se_GfxShaderHandle_t& outShaderModuleHandle);

SAVANNA_EXPORT(se_JobHandle_t) SavannaGfxCreateShaderModulesAsync(
    const se_GfxShaderCreateInfo_t* pCreateInfos,
    const size_t createInfoCount,
    se_GfxShaderHandle_t** const ppOutShaderModuleHandles);

/**
 * @brief Gets the capabilities of the graphics system. Is valid even before
 *      SavannaGfxInit has been called though the returned capabilities may
 *      not be completely filled out until after SavannaGfxContextCreateDriver
 *      has been called.
 */
SAVANNA_EXPORT(const se_GfxCapabilities_t) SavannaGfxGetCapabilities();

#endif // !I_SAVANNA_GFX_H
