#pragma once

#include "SavannaEngine.h"

namespace Savanna::Gfx
{
    class Device
    {
    private:
        Device(const Device& other) = delete;
        Device(Device&& other) = delete;

    protected:
        Device() = default;

    public:
        virtual ~Device() = default;

        virtual void Reinitialize() = 0;
        virtual void* GetPhysicalDeviceHandle() = 0;
        virtual void* GetLogicalDeviceHandle() = 0;
    };
} // namespace Savanna::Gfx
