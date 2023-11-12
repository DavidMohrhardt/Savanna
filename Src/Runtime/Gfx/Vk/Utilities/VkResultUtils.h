#pragma once

#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>

namespace Savanna::Gfx::Vk2
{
    inline constexpr const char* ResultToString(VkResult result)
    {
        return string_VkResult(result);
    }

#define ERROR_CODE_ENTRY(__vkResult, __seErrorCode) \
    case __vkResult: \
        return kSavannaGfxErrorCode##__seErrorCode

    inline constexpr se_GfxErrorCode_t ResultToErrorCode(VkResult result)
    {
        switch (result)
        {
            ERROR_CODE_ENTRY(
                VK_SUCCESS, Success);
            ERROR_CODE_ENTRY(
                VK_NOT_READY, NotInitialized);
            ERROR_CODE_ENTRY(
                VK_TIMEOUT, PlatformSpecificError);
            ERROR_CODE_ENTRY(
                VK_EVENT_SET, PlatformSpecificError);
            ERROR_CODE_ENTRY(
                VK_EVENT_RESET, PlatformSpecificError);
            ERROR_CODE_ENTRY(
                VK_INCOMPLETE, PlatformSpecificError);
            ERROR_CODE_ENTRY(
                VK_ERROR_OUT_OF_HOST_MEMORY, OutOfMemory);
            ERROR_CODE_ENTRY(
                VK_ERROR_OUT_OF_DEVICE_MEMORY, OutOfDeviceMemory);
            ERROR_CODE_ENTRY(
                VK_ERROR_INITIALIZATION_FAILED, NotInitialized);
            ERROR_CODE_ENTRY(
                VK_ERROR_DEVICE_LOST, PlatformSpecificError);
            ERROR_CODE_ENTRY(
                VK_ERROR_MEMORY_MAP_FAILED, PlatformSpecificError);
            ERROR_CODE_ENTRY(
                VK_ERROR_LAYER_NOT_PRESENT, PlatformSpecificError);
            ERROR_CODE_ENTRY(
                VK_ERROR_EXTENSION_NOT_PRESENT, PlatformSpecificError);
            ERROR_CODE_ENTRY(
                VK_ERROR_FEATURE_NOT_PRESENT, PlatformSpecificError);
            ERROR_CODE_ENTRY(
                VK_ERROR_INCOMPATIBLE_DRIVER, PlatformSpecificError);
            ERROR_CODE_ENTRY(
                VK_ERROR_TOO_MANY_OBJECTS, PlatformSpecificError);
            ERROR_CODE_ENTRY(
                VK_ERROR_FORMAT_NOT_SUPPORTED, PlatformSpecificError);
            ERROR_CODE_ENTRY(
                VK_ERROR_FRAGMENTED_POOL, PlatformSpecificError);
            ERROR_CODE_ENTRY(
                VK_ERROR_UNKNOWN, UnknownError);
            ERROR_CODE_ENTRY(
                VK_ERROR_OUT_OF_POOL_MEMORY, OutOfMemory);
            ERROR_CODE_ENTRY(
                VK_ERROR_INVALID_EXTERNAL_HANDLE, PlatformSpecificError);
            ERROR_CODE_ENTRY(
                VK_ERROR_FRAGMENTATION, PlatformSpecificError);
            ERROR_CODE_ENTRY(
                VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS, PlatformSpecificError);
            ERROR_CODE_ENTRY(
                VK_PIPELINE_COMPILE_REQUIRED, PlatformSpecificError);
            ERROR_CODE_ENTRY(
                VK_ERROR_SURFACE_LOST_KHR, PlatformSpecificError);
            ERROR_CODE_ENTRY(
                VK_ERROR_NATIVE_WINDOW_IN_USE_KHR, PlatformSpecificError);
            ERROR_CODE_ENTRY(
                VK_SUBOPTIMAL_KHR, PlatformSpecificError);
            ERROR_CODE_ENTRY(
                VK_ERROR_OUT_OF_DATE_KHR, PlatformSpecificError);
            ERROR_CODE_ENTRY(
                VK_ERROR_INCOMPATIBLE_DISPLAY_KHR, PlatformSpecificError);
            ERROR_CODE_ENTRY(
                VK_ERROR_VALIDATION_FAILED_EXT, PlatformSpecificError);
            ERROR_CODE_ENTRY(
                VK_ERROR_INVALID_SHADER_NV, PlatformSpecificError);
            ERROR_CODE_ENTRY(
                VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR, PlatformSpecificError);
            ERROR_CODE_ENTRY(
                VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR, PlatformSpecificError);
            ERROR_CODE_ENTRY(
                VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR, PlatformSpecificError);
            ERROR_CODE_ENTRY(
                VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR, PlatformSpecificError);
            ERROR_CODE_ENTRY(
                VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR, PlatformSpecificError);
            ERROR_CODE_ENTRY(
                VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR, PlatformSpecificError);
            ERROR_CODE_ENTRY(
                VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT, PlatformSpecificError);
            ERROR_CODE_ENTRY(
                VK_ERROR_NOT_PERMITTED_KHR, PlatformSpecificError);
            ERROR_CODE_ENTRY(
                VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT, PlatformSpecificError);
            ERROR_CODE_ENTRY(
                VK_THREAD_IDLE_KHR, PlatformSpecificError);
            ERROR_CODE_ENTRY(
                VK_THREAD_DONE_KHR, PlatformSpecificError);
            ERROR_CODE_ENTRY(
                VK_OPERATION_DEFERRED_KHR, PlatformSpecificError);
            ERROR_CODE_ENTRY(
                VK_OPERATION_NOT_DEFERRED_KHR, PlatformSpecificError);
#ifdef VK_ENABLE_BETA_EXTENSIONS
            ERROR_CODE_ENTRY(
                VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR, PlatformSpecificError);
#endif
            ERROR_CODE_ENTRY(
                VK_ERROR_COMPRESSION_EXHAUSTED_EXT, PlatformSpecificError);
            // The following entries are repeats
            // ERROR_CODE_ENTRY(
                // VK_ERROR_OUT_OF_POOL_MEMORY_KHR, PlatformSpecificError);
            // ERROR_CODE_ENTRY(
                // VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR, PlatformSpecificError);
            // ERROR_CODE_ENTRY(
                // VK_ERROR_FRAGMENTATION_EXT, PlatformSpecificError);
            // ERROR_CODE_ENTRY(
                // VK_ERROR_NOT_PERMITTED_EXT, PlatformSpecificError);
            // ERROR_CODE_ENTRY(
                // VK_ERROR_INVALID_DEVICE_ADDRESS_EXT, PlatformSpecificError);
            // ERROR_CODE_ENTRY(
                // VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS_KHR, PlatformSpecificError);
            // ERROR_CODE_ENTRY(
                // VK_PIPELINE_COMPILE_REQUIRED_EXT, PlatformSpecificError);
            // ERROR_CODE_ENTRY(
                // VK_ERROR_PIPELINE_COMPILE_REQUIRED_EXT, PlatformSpecificError);
            ERROR_CODE_ENTRY(
                VK_RESULT_MAX_ENUM, Failure);
        }
    }
} // namespace Vk

#undef ERROR_CODE_ENTRY
