/**
 * @file SavannaGfx.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-09-19
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef I_SAVANNA_GFX_H
#define I_SAVANNA_GFX_H

#include "Public/ISavannaEngine.h"

#undef SAVANNA_GFX_SUCCESS
#define SAVANNA_GFX_SUCCESS(__operationOrValue) \
    ((__operationOrValue) == kSavannaGfxErrorCodeSuccess)

#undef SAVANNA_GFX_FAILURE
#define SAVANNA_GFX_FAILURE(__operationOrValue) \
    ((__operationOrValue) >= kSavannaGfxErrorCodeFailure)

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
     * @brief There was an unknown error.
     */
    kSavannaGfxErrorCodeUnknownError,

    /**
     * @brief There was not enough memory to complete the operation.
    */
    kSavannaGfxErrorCodeOutOfMemory,

    /**
     * @brief The function threw an exception.
     */
    kSavannaGfxErrorCodeException,

    /**
     * @brief The function was provided an invalid argument.
     */
    kSavannaGfxErrorCodeInvalidArgument,

    /**
     * @brief The graphics context has not been initialized.
     */
    kSavannaGfxErrorCodeGfxNotInitialized,

    /**
     * @brief The graphics was not able to create a IGfxDriver.
     */
    kSavannaGfxErrorCodeUnsupportedGfxBackend,

    /**
     * @brief The graphics driver was unable to create a graphics driver.
     */
    kSavannaGfxErrorCodeUnableToCreateGfxDriver,

    /**
     * @brief The graphics driver has already been created.
     * @note This error code may not always be considered an error.
     */
    kSavannaGfxErrorCodeGfxDriverAlreadyCreated,
} se_GfxErrorCode_t;

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

typedef enum se_GfxSupportedBackend_t : se_uint8
{
    kSavannaSupportedGfxApiNone = kSavannaGfxApiNone,
    kSavannaSupportedGfxApiAll = static_cast<se_uint8>(~0),

    kSavannaSupportedGfxApiVulkan = 1 << kSavannaGfxApiVulkan,
    // kSavannaSupportedGfxApiDirectX12 = 1 << kSavannaGfxApiDirectX12,
    // kSavannaSupportedGfxApiDirectX11 = 1 << kSavannaGfxApiDirectX11,
    // kSavannaSupportedGfxApiOpenGL = 1 << kSavannaGfxApiOpenGL,
    // kSavannaSupportedGfxApiOpenGLES = 1 << kSavannaGfxApiOpenGLES,
    // kSavannaSupportedGfxApiMetal = 1 << kSavannaGfxApiMetal,
} se_GfxSupportedBackend_t;

/// Graphics Context Section
typedef se_BeforeAfterCallbacks_t se_GfxContextCreationCallbacks_t;

typedef struct se_GfxContextCreateInfo_t
{
    se_AllocatorInterface_t m_Allocator;

    se_GfxContextCreationCallbacks_t m_CreationCallbacks;

    void* m_pUserData;
} se_GfxContextCreateInfo_t;

/// Graphics Driver Section
typedef struct se_GfxDriverCreateInfo_t
{
    /**
     * @brief The type of backend to create.
     */
    se_GfxBackend_t m_RequestedBackendType;

    /**
     * @brief The allocator the driver should use.
     */
    se_AllocatorInterface_t m_Allocator;

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

/**
 * @brief A function pointer defining the function signature for creating a graphics driver.
 */
typedef se_GfxErrorCode_t (*se_pfnGfxDriverCreate_t)(const se_GfxDriverCreateInfo_t& pCreateInfo);

/**
 * @brief A function pointer defining the function signature for destroying a graphics driver.
 */
typedef se_GfxErrorCode_t (*se_pfnGfxDriverDestroy_t)();

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
    se_pfnGfxDriverCreate_t m_pfnCreate;

    /**
     * @brief A pointer to the function for destroying a graphics driver.
     */
    se_pfnGfxDriverDestroy_t m_pfnDestroy;

    /**
     * @brief
     *
     */
    void* m_pUserData;
} se_GfxDriverInterface_t;

typedef intptr_t se_GfxHandle_t;

#define SE_GFX_INVALID_HANDLE static_cast<se_GfxHandle_t>(0)

#define SE_GFX_HANDLE_TYPEDEF(__handleType) \
    typedef se_GfxHandle_t __handleType##Handle_t;

/**
 * @brief A handle to a graphics context. There can only be zero or one graphics context.
 *
 */
SE_GFX_HANDLE_TYPEDEF(se_GfxContext);

/**
 * @brief A handle to a graphics driver. There can only be zero or one graphics driver.
 *
 */
SE_GFX_HANDLE_TYPEDEF(se_GfxDriver);

/**
 * @brief A handle to a native command buffer. This is a handle to the native graphics API.
 *
 */
SE_GFX_HANDLE_TYPEDEF(se_GfxCommandBuffer);

SE_GFX_HANDLE_TYPEDEF(se_GfxFence);

SE_GFX_HANDLE_TYPEDEF(se_GfxSemaphore);

SE_GFX_HANDLE_TYPEDEF(se_GfxBuffer);

SE_GFX_HANDLE_TYPEDEF(se_GfxImage);

SE_GFX_HANDLE_TYPEDEF(se_GfxImageView);

SE_GFX_HANDLE_TYPEDEF(se_GfxSampler);

SE_GFX_HANDLE_TYPEDEF(se_GfxShaderModule);

SE_GFX_HANDLE_TYPEDEF(se_GfxPipelineLayout);

SE_GFX_HANDLE_TYPEDEF(se_GfxRenderPass);

SE_GFX_HANDLE_TYPEDEF(se_GfxPipeline);

SE_GFX_HANDLE_TYPEDEF(se_GfxDescriptorSetLayout);

SE_GFX_HANDLE_TYPEDEF(se_GfxDescriptorPool);

SE_GFX_HANDLE_TYPEDEF(se_GfxDescriptorSet);

SE_GFX_HANDLE_TYPEDEF(se_GfxFramebuffer);

#undef SE_GFX_HANDLE_TYPEDEF


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

#endif // !I_SAVANNA_GFX_H
