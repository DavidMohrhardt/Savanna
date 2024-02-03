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
    static seGfxErrorCode Initialize(const seGfxDriverCreateInfo& createInfo); \
    static seGfxErrorCode Destroy(); \
    static seGfxDriver GetDriver(); \
    static seGfxErrorCode CreateSwapchain(const seGfxSwapchainCreateInfo& createInfo, seGfxHandle* const pOutSwapchainHandle); \
    static seGfxErrorCode CreateShaderModule( \
        const seGfxShaderCreateInfo& createInfo, \
        seGfxShader& outShaderModuleHandle); \
    static seJobHandle CreateShaderModulesAsync( \
        const seGfxShaderCreateInfo* pCreateInfos, \
        const size_t createInfoCount, \
        seGfxShader** const ppOutShaderModuleHandles)

namespace savanna::gfx
{
} // namespace savanna::Gfx


#endif
