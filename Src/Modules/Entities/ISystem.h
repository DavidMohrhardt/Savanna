/**
 * @file System.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-02
 *
 */

#pragma once

#include <SavannaEngine.h>

#include "Entities/Entity.h"
#include "Entities/IComponentData.h"

namespace Savanna::Entities
{
    enum SavannaSystemErrorFlags
    {
        kSavannaSystemInvalidCode       = 0x0,
        kSavannaSystemSuccessCode       = 0x1,
        kSavannaSystemMalformedInput    = 0x2,
        kSavannaSystemEnsure32Bits      = 0xFFFFFFFF
    };

    struct SavannaSystemErrorCode
    {
        int32 m_Flags = 0x0;

        virtual bool HasFlag(int32 flag)
        {
            return ((m_Flags & flag) == flag);
        }
    };

    enum SavannaSystemOperatingHints
    {
        kSystemThreadSafe = 0x1,
        kLongRunningSystem = 0x1 << 1,
        kUninterruptable = 0x1 << 2,
        kEnsure32Bits = 0xFFFFFFFF
    };

    class ISystem
    {
    public:
        ISystem(uint64 systemHandle, const SavannaSystemOperatingHints options)
            : m_SystemHandle(systemHandle)
            , m_Options(options)
        {}

        // Disallow copy constructors
        ISystem(ISystem &other) = delete;

        virtual ~ISystem() {}

        virtual SavannaSystemErrorCode RunSystem(IComponent* components) = 0;

    protected:
        uint64 m_SystemHandle;
        SavannaSystemOperatingHints m_Options;
    };
}
