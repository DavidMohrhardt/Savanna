/**
 * @file ISavannaGfx.hpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-11-12
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef I_SAVANNA_GFX_HPP
#define I_SAVANNA_GFX_HPP

#include "ISavannaGfx.h"

/**
 * @brief A helper macro for defining the driver interface functions.
 * ensures that each interface function is declared with the correct signature.
 */
#define DECL_GFX_DRIVER_INTERFACE_FUNCS() \
    static se_GfxErrorCode_t Initialize(const se_GfxDriverCreateInfo_t& createInfo); \
    static se_GfxErrorCode_t Destroy(); \
    static se_GfxDriverHandle_t GetDriverHandle(); \
    static se_GfxErrorCode_t CreateSwapchain(const se_GfxSwapchainCreateInfo_t& createInfo, se_GfxHandle_t* const pOutSwapchainHandle); \
    static se_GfxErrorCode_t CreateShaderModule( \
        const se_GfxShaderCreateInfo_t& createInfo, \
        se_GfxShaderHandle_t& outShaderModuleHandle); \
    static se_JobHandle_t CreateShaderModulesAsync( \
        const se_GfxShaderCreateInfo_t* pCreateInfos, \
        const size_t createInfoCount, \
        se_GfxShaderHandle_t** const ppOutShaderModuleHandles)

namespace savanna::Gfx
{

} // namespace savanna::Gfx


#endif
