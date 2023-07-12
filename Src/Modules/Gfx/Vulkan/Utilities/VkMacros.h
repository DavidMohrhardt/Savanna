#pragma once

#include <vulkan/vulkan.h>
#include <stdexcept>

#include <Types/Exceptions/SavannaException.h>
#include <Utilities/Console.h>

#include "VkResultUtils.h"

// Use to call vulkan functions that should not fail at runtime.
#define VK_CALL(__func) \
    vk##__func; \

#define VK_CALL_OR_THROW(__func, __msg, ...) \
{ \
    auto __result__ = VK_CALL(__func); \
    if (__result__ != VK_SUCCESS) \
    { \
        SAVANNA_FATAL_LOG("Vulkan Runtime Error: vk" #__func " returned result {}.", Savanna::Gfx::Vk::ResultToString(__result__)); \
        throw Savanna::RuntimeErrorException(__msg); \
    } \
}

#define VK_CHECK(__outResult, __func, __msg) \
    __outResult = __func; \
    if (__outResult != VK_SUCCESS)  \
    { \
        SAVANNA_WARNING_LOG("Vulkan Runtime Error: " #__func " returned result {}. {}", Savanna::Vk::ResultToString(__outResult), msg); \
    }
