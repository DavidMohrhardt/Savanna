#pragma once

#include <vulkan/vulkan.h>
#include <stdexcept>

#include <Types/Exceptions/SavannaException.h>
#include <Utilities/Console.h>

#include "VkResultUtils.h"

// Use to call vulkan functions that should not fail at runtime.
#define VK_CALL_OR_THROW(func, msg, ...) \
{ \
    auto __result__ = func; \
    if (__result__ != VK_SUCCESS) \
    { \
        SAVANNA_FATAL_LOG("Vk Runtime Error: " #func " returned result %s. %s", Savanna::Vk::VkResultUtils::ToString(__result__), msg); \
        throw Savanna::RuntimeErrorException(msg); \
    } \
}

#define VK_CALL(result, func, msg) \
    result = func; \
    if (result != VK_SUCCESS)  \
    { \
        SAVANNA_WARNING_LOG("Vk Runtime Error: " #func " returned result %s. %s", Savanna::Vk::VkResultUtils::ToString(result), msg); \
    }
