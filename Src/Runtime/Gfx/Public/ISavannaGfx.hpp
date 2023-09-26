/**
 * @file ISavannaGfx.hpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-09-23
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef I_SAVANNA_GFX_HPP
#define I_SAVANNA_GFX_HPP

#include "ISavannaGfx.h"

namespace Savanna::Gfx
{
    using GfxErrorCode = Enumeration<se_GfxErrorCode_t, se_uint32>;
    using GfxBackend = Enumeration<se_GfxBackend_t, se_uint32>;
    using GfxSupportedBackend = FlagEnumeration<se_GfxSupportedBackend_t, se_uint8>;

    const se_AllocatorInterface_t GetDefaultAllocatorInterface();

    GfxErrorCode Initialize(const se_GfxContextCreateInfo_t* const pCreateInfo);
    GfxErrorCode Shutdown();

    GfxErrorCode CreateDriver(const se_GfxDriverCreateInfoList_t* const pCreateInfoList);
}

#endif // !I_SAVANNA_GFX_HPP
