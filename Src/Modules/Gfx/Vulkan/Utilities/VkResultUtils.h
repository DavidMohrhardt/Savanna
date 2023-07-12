#pragma once

#include <vulkan/vulkan.h>

#include "VkDefinitions.h"

namespace Savanna::Gfx::Vk
{
#define CASE_RETURN_STRING(x) \
    case x:                   \
        return #x

    constexpr const char* ResultToString(VkResult result)
    {
        switch (result)
        {
            CASE_RETURN_STRING(VK_SUCCESS);
            CASE_RETURN_STRING(VK_NOT_READY);
            CASE_RETURN_STRING(VK_TIMEOUT);
            CASE_RETURN_STRING(VK_EVENT_SET);
            CASE_RETURN_STRING(VK_EVENT_RESET);
            CASE_RETURN_STRING(VK_INCOMPLETE);
            CASE_RETURN_STRING(VK_ERROR_OUT_OF_HOST_MEMORY);
            CASE_RETURN_STRING(VK_ERROR_OUT_OF_DEVICE_MEMORY);
            CASE_RETURN_STRING(VK_ERROR_INITIALIZATION_FAILED);
            CASE_RETURN_STRING(VK_ERROR_DEVICE_LOST);
            CASE_RETURN_STRING(VK_ERROR_MEMORY_MAP_FAILED);
            CASE_RETURN_STRING(VK_ERROR_LAYER_NOT_PRESENT);
            CASE_RETURN_STRING(VK_ERROR_EXTENSION_NOT_PRESENT);
            CASE_RETURN_STRING(VK_ERROR_FEATURE_NOT_PRESENT);
            CASE_RETURN_STRING(VK_ERROR_INCOMPATIBLE_DRIVER);
            CASE_RETURN_STRING(VK_ERROR_TOO_MANY_OBJECTS);
            CASE_RETURN_STRING(VK_ERROR_FORMAT_NOT_SUPPORTED);
            CASE_RETURN_STRING(VK_ERROR_FRAGMENTED_POOL);
            CASE_RETURN_STRING(VK_ERROR_OUT_OF_POOL_MEMORY);
            CASE_RETURN_STRING(VK_ERROR_INVALID_EXTERNAL_HANDLE);
            CASE_RETURN_STRING(VK_ERROR_FRAGMENTATION);
            CASE_RETURN_STRING(VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS);
            CASE_RETURN_STRING(VK_ERROR_SURFACE_LOST_KHR);
            CASE_RETURN_STRING(VK_ERROR_NATIVE_WINDOW_IN_USE_KHR);
            CASE_RETURN_STRING(VK_SUBOPTIMAL_KHR);
            CASE_RETURN_STRING(VK_ERROR_OUT_OF_DATE_KHR);
            CASE_RETURN_STRING(VK_ERROR_INCOMPATIBLE_DISPLAY_KHR);
            CASE_RETURN_STRING(VK_ERROR_VALIDATION_FAILED_EXT);
            CASE_RETURN_STRING(VK_ERROR_INVALID_SHADER_NV);
            CASE_RETURN_STRING(VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT);
            CASE_RETURN_STRING(VK_ERROR_NOT_PERMITTED_EXT);
            CASE_RETURN_STRING(VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT);
            CASE_RETURN_STRING(VK_THREAD_IDLE_KHR);
            CASE_RETURN_STRING(VK_THREAD_DONE_KHR);
            CASE_RETURN_STRING(VK_OPERATION_DEFERRED_KHR);
            CASE_RETURN_STRING(VK_OPERATION_NOT_DEFERRED_KHR);
            CASE_RETURN_STRING(VK_PIPELINE_COMPILE_REQUIRED_EXT);
            CASE_RETURN_STRING(VK_RESULT_MAX_ENUM);
            default:
            CASE_RETURN_STRING(VK_ERROR_UNKNOWN);

            // The following are redefines of existing values for backwards compatibility
            // CASE_RETURN_STRING(VK_ERROR_OUT_OF_POOL_MEMORY_KHR);
            // CASE_RETURN_STRING(VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR);
            // CASE_RETURN_STRING(VK_ERROR_FRAGMENTATION_EXT);
            // CASE_RETURN_STRING(VK_ERROR_INVALID_DEVICE_ADDRESS_EXT);
            // CASE_RETURN_STRING(VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS_KHR);
            // CASE_RETURN_STRING(VK_ERROR_PIPELINE_COMPILE_REQUIRED_EXT);
        }
    }
} // namespace Vk

#undef CASE_RETURN_STRING
