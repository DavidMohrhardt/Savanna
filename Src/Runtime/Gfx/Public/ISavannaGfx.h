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

#include "SavannaEngine.h"
#include "Utilities/SavannaCoding.h"

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
     *
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
     *
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

typedef enum se_GfxSupportedGfxBackend_t : se_uint8
{
    kSavannaSupportedGfxApiNone = kSavannaGfxApiNone,
    kSavannaSupportedGfxApiAll = static_cast<se_uint8>(~0),

    kSavannaSupportedGfxApiVulkan = 1 << kSavannaGfxApiVulkan,
    // kSavannaSupportedGfxApiDirectX12 = 1 << kSavannaGfxApiDirectX12,
    // kSavannaSupportedGfxApiDirectX11 = 1 << kSavannaGfxApiDirectX11,
    // kSavannaSupportedGfxApiOpenGL = 1 << kSavannaGfxApiOpenGL,
    // kSavannaSupportedGfxApiOpenGLES = 1 << kSavannaGfxApiOpenGLES,
    // kSavannaSupportedGfxApiMetal = 1 << kSavannaGfxApiMetal,
} se_GfxSupportedGfxBackend_t;

/// Graphics Context Section
typedef struct se_GfxContextCreateInfo_t
{
    se_AllocatorInterface_t m_Allocator;
    void* m_pUserData;
} se_GfxContextCreateInfo_t;

/// Graphics Driver Section
typedef struct se_GfxDriverCreateInfo_t
{
    se_GfxBackend_t m_RequestedBackendType;
    se_AllocatorInterface_t m_Allocator;

    void* m_pRealDriverCreateInfo;
    void* m_pUserData;
} se_GfxDriverCreateInfo_t;

typedef struct se_GfxDriverCreateInfoList_t
{
    se_GfxDriverCreateInfo_t* m_pDriverCreateInfos;
    se_uint32 m_CreateInfoCount;

    void* m_pUserData;
} se_GfxDriverCreateInfoList_t;

/**
 * @brief
 *
 */
typedef se_GfxErrorCode_t (*se_pfnGfxDriverCreate_t)(const se_GfxDriverCreateInfo_t& pCreateInfo);

/**
 * @brief
 *
 */
typedef void (*se_pfnGfxDriverDestroy_t)();

/**
 * @brief
 *
 */
typedef struct se_GfxDriverInterface_t
{
    se_pfnGfxDriverCreate_t m_pfnCreate;
    se_pfnGfxDriverDestroy_t m_pfnDestroy;

    void* m_pUserData;
} se_GfxDriverInterface_t;

/// API Section

#if defined(__cplusplus)
extern "C"
{
#endif // defined(__cplusplus)

    SAVANNA_EXPORT(se_GfxErrorCode_t) SavannaGfxInit(const se_GfxContextCreateInfo_t* const pCreateInfo);
    SAVANNA_EXPORT(se_GfxErrorCode_t) SavannaGfxShutdown();

    SAVANNA_EXPORT(se_GfxErrorCode_t) SavannaGfxContextCreateDriver(
        const se_GfxDriverCreateInfoList_t* const pCreateInfoList);

    SAVANNA_EXPORT(se_GfxSupportedGfxBackend_t) SavannaGfxGetSupportedGraphicsBackends();
    SAVANNA_EXPORT(se_GfxBackend_t) SavannaGfxGetActiveGraphicsApi();

#if defined(__cplusplus)
}
#endif // defined(__cplusplus)
#endif // !I_SAVANNA_GFX_H
