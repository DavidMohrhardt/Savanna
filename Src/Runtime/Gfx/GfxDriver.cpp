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
    static consteval size_t GetGfxDriverInterfaceSizeInPtrs()
    {
        return sizeof(se_GfxDriverInterface_t) / sizeof(void*);
    }

    bool GfxDriver::IsValid()
    {

        for (int i = 0; i < GetGfxDriverInterfaceSizeInPtrs(); ++i)
        {
            if (reinterpret_cast<const uint8_t*>(&m_Interface)[i] == 0)
            {
                return false;
            }
        }
        return true;
    }
} // namespace Savanna::Gfx
