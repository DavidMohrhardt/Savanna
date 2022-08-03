#pragma once

#include "GfxDeviceErrCode.h"
#include "GfxApiEnumeration.h"

namespace Savanna
{
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