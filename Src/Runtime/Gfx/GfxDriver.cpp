/**
 * @file GfxDriver.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-09-23
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "GfxDriver.h"

namespace Savanna::Gfx
{
    se_GfxErrorCode_t GenericGfxDriver::CreateDriver(
        const se_GfxDriverCreateInfo_t& createInfo,
        IGfxDriver** ppDriver,
        void* pUserData)
    {
        return kSavannaGfxErrorCodeUnknownError;
    }
} // namespace Savanna::Gfx
