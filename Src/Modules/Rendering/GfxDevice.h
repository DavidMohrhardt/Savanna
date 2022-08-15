/**
 * @file GfxDevice.h
 * @author David Mohrhardt (DavidMohrhardt@github.com)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-02
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include "GfxDeviceErrCode.h"
#include "GfxApiEnumeration.h"

namespace Savanna
{
    /**
     * @brief TODO @DavidMohrhardt Document
     *
     */
    class GfxDevice
    {
    public:
        virtual GfxDeviceErrCode Initialize() = 0;
        virtual GfxDeviceErrCode Deinitilaize() = 0;

        const GfxType GetDeviceType() { return m_CurrentGfxDeviceAPI; }

    protected:
        GfxType m_CurrentGfxDeviceAPI;

    private:

    };
}
