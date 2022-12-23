#pragma once

#include <vulkan/vulkan.h>
#include <stdexcept>

#include <Types/Exceptions/SavannaException.h>
#include <Utilities/Console.h>

#include "VulkanResultUtils.h"

// Use to call vulkan functions that should not fail at runtime.
#define VK_CALL_OR_THROW(func, msg, ...) \
{ \
    auto __result__ = func; \
    if (__result__ != VK_SUCCESS) \
    { \
        SAVANNA_FATAL_LOG("Vulkan Runtime Error: " #func " returned result %s. %s", Savanna::Vulkan::VulkanResultUtils::ToString(__result__), msg); \
        throw Savanna::RuntimeErrorException(msg); \
    } \
}

#define VK_CALL(result, func, msg) \
    result = func; \
    if (result != VK_SUCCESS)  \
    { \
        SAVANNA_WARNING_LOG("Vulkan Runtime Error: " #func " returned result %s. %s", Savanna::Vulkan::VulkanResultUtils::ToString(result), msg); \
    }
