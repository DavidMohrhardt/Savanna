/**
 * @file VulkanPhysicalDevice.cpp
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @DavidMohrhardt Document
 * @version 0.1
 * @date 2022-08-02
 *
 */

#include "VkPhysicalDeviceUtils.h"

#include "VkRendererCreateInfo.h"

#include <Profiling/Profiler.h>

#include <cstdlib>
#include <mutex>
#include <assert.h>
#include <vector>
#include <stdexcept>

namespace Savanna::Gfx::Vk::Utils
{
    uint32 GetPhysicalDeviceCount(const VkInstance& instance)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(Savanna::Gfx::Vk::GetPhysicalDeviceCount);
        static uint32 s_NumberOfDevices = 0;
        // Should only be true once per run.
        if (s_NumberOfDevices == 0) SAVANNA_BRANCH_UNLIKELY
        {
            uint32 count = 0;
            vkEnumeratePhysicalDevices(instance, &count, nullptr);
            s_NumberOfDevices = count;
        }
        return s_NumberOfDevices;
    }

    inline void GetPhysicalDeviceDescriptors(
        const VkInstance& instance,
        uint32 count,
        PhysicalDeviceDescriptor* physicalDeviceDescriptorsPtr)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(Savanna::Gfx::Vk::GetPhysicalDeviceDescriptors);
        if (physicalDeviceDescriptorsPtr != nullptr)
        {
            std::vector<VkPhysicalDevice> physicalDevices(count);
            vkEnumeratePhysicalDevices(instance, &count, physicalDevices.data());
            for (int i = 0; i < count; ++i)
            {
                physicalDeviceDescriptorsPtr[i].physicalDevice = physicalDevices[i];
                vkGetPhysicalDeviceProperties(physicalDevices[i], &physicalDeviceDescriptorsPtr[i].properties);
                vkGetPhysicalDeviceFeatures(physicalDevices[i], &physicalDeviceDescriptorsPtr[i].features);
                vkGetPhysicalDeviceMemoryProperties(physicalDevices[i], &physicalDeviceDescriptorsPtr[i].memoryProperties);
            }
        }
    }

    void EnumeratePhysicalDeviceDescriptors(
        const VkInstance& instance,
        PhysicalDeviceDescriptor* physicalDeviceDescriptorsPtr,
        uint32& count)
    {
        SAVANNA_INSERT_SCOPED_PROFILER(Savanna::Gfx::Vk::EnumeratePhysicalDeviceDescriptors);
        count = GetPhysicalDeviceCount(instance);
        if (physicalDeviceDescriptorsPtr != nullptr)
        {
            GetPhysicalDeviceDescriptors(instance, count, physicalDeviceDescriptorsPtr);
        }
    }

    bool ScoreDeviceType(const VkPhysicalDeviceType& deviceType, uint32& outScore)
    {
        switch (deviceType)
        {
        case VK_PHYSICAL_DEVICE_TYPE_CPU:
            outScore = 10;
            break;
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
            outScore = 100;
            break;
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
            outScore = 500;
            break;
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
            outScore = 1000;
            break;
        default:
            return false;
        }

        return true;
    }

    bool ScoreDeviceFeatures(const VkPhysicalDeviceFeatures& deviceFeatures, uint32& outScore)
    {
        // Grabbed from https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkPhysicalDeviceFeatures.html

        // TODO @DavidMohrhardt Just arbitrarily add 1 for everything and fine tune later
        outScore = 0;
        if (deviceFeatures.robustBufferAccess) outScore += 1;
        if (deviceFeatures.fullDrawIndexUint32) outScore += 1;
        if (deviceFeatures.imageCubeArray) outScore += 1;
        if (deviceFeatures.independentBlend) outScore += 1;
        if (deviceFeatures.geometryShader) outScore += 1;
        if (deviceFeatures.tessellationShader) outScore += 1;
        if (deviceFeatures.sampleRateShading) outScore += 1;
        if (deviceFeatures.dualSrcBlend) outScore += 1;
        if (deviceFeatures.logicOp) outScore += 1;
        if (deviceFeatures.multiDrawIndirect) outScore += 1;
        if (deviceFeatures.drawIndirectFirstInstance) outScore += 1;
        if (deviceFeatures.depthClamp) outScore += 1;
        if (deviceFeatures.depthBiasClamp) outScore += 1;
        if (deviceFeatures.fillModeNonSolid) outScore += 1;
        if (deviceFeatures.depthBounds) outScore += 1;
        if (deviceFeatures.wideLines) outScore += 1;
        if (deviceFeatures.largePoints) outScore += 1;
        if (deviceFeatures.alphaToOne) outScore += 1;
        if (deviceFeatures.multiViewport) outScore += 1;
        if (deviceFeatures.samplerAnisotropy) outScore += 1;
        if (deviceFeatures.textureCompressionETC2) outScore += 1;
        if (deviceFeatures.textureCompressionASTC_LDR) outScore += 1;
        if (deviceFeatures.textureCompressionBC) outScore += 1;
        if (deviceFeatures.occlusionQueryPrecise) outScore += 1;
        if (deviceFeatures.pipelineStatisticsQuery) outScore += 1;
        if (deviceFeatures.vertexPipelineStoresAndAtomics) outScore += 1;
        if (deviceFeatures.fragmentStoresAndAtomics) outScore += 1;
        if (deviceFeatures.shaderTessellationAndGeometryPointSize) outScore += 1;
        if (deviceFeatures.shaderImageGatherExtended) outScore += 1;
        if (deviceFeatures.shaderStorageImageExtendedFormats) outScore += 1;
        if (deviceFeatures.shaderStorageImageMultisample) outScore += 1;
        if (deviceFeatures.shaderStorageImageReadWithoutFormat) outScore += 1;
        if (deviceFeatures.shaderStorageImageWriteWithoutFormat) outScore += 1;
        if (deviceFeatures.shaderUniformBufferArrayDynamicIndexing) outScore += 1;
        if (deviceFeatures.shaderSampledImageArrayDynamicIndexing) outScore += 1;
        if (deviceFeatures.shaderStorageBufferArrayDynamicIndexing) outScore += 1;
        if (deviceFeatures.shaderStorageImageArrayDynamicIndexing) outScore += 1;
        if (deviceFeatures.shaderClipDistance) outScore += 1;
        if (deviceFeatures.shaderCullDistance) outScore += 1;
        if (deviceFeatures.shaderFloat64) outScore += 1;
        if (deviceFeatures.shaderInt64) outScore += 1;
        if (deviceFeatures.shaderInt16) outScore += 1;
        if (deviceFeatures.shaderResourceResidency) outScore += 1;
        if (deviceFeatures.shaderResourceMinLod) outScore += 1;
        if (deviceFeatures.sparseBinding) outScore += 1;
        if (deviceFeatures.sparseResidencyBuffer) outScore += 1;
        if (deviceFeatures.sparseResidencyImage2D) outScore += 1;
        if (deviceFeatures.sparseResidencyImage3D) outScore += 1;
        if (deviceFeatures.sparseResidency2Samples) outScore += 1;
        if (deviceFeatures.sparseResidency4Samples) outScore += 1;
        if (deviceFeatures.sparseResidency8Samples) outScore += 1;
        if (deviceFeatures.sparseResidency16Samples) outScore += 1;
        if (deviceFeatures.sparseResidencyAliased) outScore += 1;
        if (deviceFeatures.variableMultisampleRate) outScore += 1;
        if (deviceFeatures.inheritedQueries) outScore += 1;
        return outScore > 0;
    }

    bool ScoreDeviceLimits(const VkPhysicalDeviceLimits& deviceLimits, uint32& outScore)
    {
        // From https://registry.khronos.org/vulkan/specs/1.3-extensions/man/html/VkPhysicalDeviceLimits.html#:~:text=The%20VkPhysicalDeviceLimits%20are%20properties%20of%20the%20physical%20device.,VkPhysicalDeviceProperties%20structure%20which%20is%20returned%20from%20vkGetPhysicalDeviceProperties.%20Description

        // TODO @DavidMohrhardt Just arbitrarily add the deviceDescsCount for everything and fine tune later
        outScore += deviceLimits.maxImageDimension1D;
        outScore += deviceLimits.maxImageDimension2D;
        outScore += deviceLimits.maxImageDimension3D;
        outScore += deviceLimits.maxImageDimensionCube;
        outScore += deviceLimits.maxImageArrayLayers;
        outScore += deviceLimits.maxTexelBufferElements;
        outScore += deviceLimits.maxUniformBufferRange;
        outScore += deviceLimits.maxStorageBufferRange;
        outScore += deviceLimits.maxPushConstantsSize;
        outScore += deviceLimits.maxMemoryAllocationCount;
        outScore += deviceLimits.maxSamplerAllocationCount;
        // VkDeviceSize          bufferImageGranularity;
        // VkDeviceSize          sparseAddressSpaceSize;
        outScore += deviceLimits.maxBoundDescriptorSets;
        outScore += deviceLimits.maxPerStageDescriptorSamplers;
        outScore += deviceLimits.maxPerStageDescriptorUniformBuffers;
        outScore += deviceLimits.maxPerStageDescriptorStorageBuffers;
        outScore += deviceLimits.maxPerStageDescriptorSampledImages;
        outScore += deviceLimits.maxPerStageDescriptorStorageImages;
        outScore += deviceLimits.maxPerStageDescriptorInputAttachments;
        outScore += deviceLimits.maxPerStageResources;
        outScore += deviceLimits.maxDescriptorSetSamplers;
        outScore += deviceLimits.maxDescriptorSetUniformBuffers;
        outScore += deviceLimits.maxDescriptorSetUniformBuffersDynamic;
        outScore += deviceLimits.maxDescriptorSetStorageBuffers;
        outScore += deviceLimits.maxDescriptorSetStorageBuffersDynamic;
        outScore += deviceLimits.maxDescriptorSetSampledImages;
        outScore += deviceLimits.maxDescriptorSetStorageImages;
        outScore += deviceLimits.maxDescriptorSetInputAttachments;
        outScore += deviceLimits.maxVertexInputAttributes;
        outScore += deviceLimits.maxVertexInputBindings;
        outScore += deviceLimits.maxVertexInputAttributeOffset;
        outScore += deviceLimits.maxVertexInputBindingStride;
        outScore += deviceLimits.maxVertexOutputComponents;
        outScore += deviceLimits.maxTessellationGenerationLevel;
        outScore += deviceLimits.maxTessellationPatchSize;
        outScore += deviceLimits.maxTessellationControlPerVertexInputComponents;
        outScore += deviceLimits.maxTessellationControlPerVertexOutputComponents;
        outScore += deviceLimits.maxTessellationControlPerPatchOutputComponents;
        outScore += deviceLimits.maxTessellationControlTotalOutputComponents;
        outScore += deviceLimits.maxTessellationEvaluationInputComponents;
        outScore += deviceLimits.maxTessellationEvaluationOutputComponents;
        outScore += deviceLimits.maxGeometryShaderInvocations;
        outScore += deviceLimits.maxGeometryInputComponents;
        outScore += deviceLimits.maxGeometryOutputComponents;
        outScore += deviceLimits.maxGeometryOutputVertices;
        outScore += deviceLimits.maxGeometryTotalOutputComponents;
        outScore += deviceLimits.maxFragmentInputComponents;
        outScore += deviceLimits.maxFragmentOutputAttachments;
        outScore += deviceLimits.maxFragmentDualSrcAttachments;
        outScore += deviceLimits.maxFragmentCombinedOutputResources;
        outScore += deviceLimits.maxComputeSharedMemorySize;
        // outScore += deviceLimits.maxComputeWorkGroupCount[3];
        outScore += deviceLimits.maxComputeWorkGroupInvocations;
        // outScore += deviceLimits.maxComputeWorkGroupSize[3];
        outScore += deviceLimits.subPixelPrecisionBits;
        outScore += deviceLimits.subTexelPrecisionBits;
        outScore += deviceLimits.mipmapPrecisionBits;
        outScore += deviceLimits.maxDrawIndexedIndexValue;
        outScore += deviceLimits.maxDrawIndirectCount;
        // float                 maxSamplerLodBias;
        // float                 maxSamplerAnisotropy;
        outScore += deviceLimits.maxViewports;
        // outScore += deviceLimits.maxViewportDimensions[2];
        // float                 viewportBoundsRange[2];
        outScore += deviceLimits.viewportSubPixelBits;
        // size_t                minMemoryMapAlignment;
        // VkDeviceSize          minTexelBufferOffsetAlignment;
        // VkDeviceSize          minUniformBufferOffsetAlignment;
        // VkDeviceSize          minStorageBufferOffsetAlignment;
        // int32_t               minTexelOffset;
        // outScore += deviceLimits.maxTexelOffset;
        // int32_t               minTexelGatherOffset;
        // outScore += deviceLimits.maxTexelGatherOffset;
        // float                 minInterpolationOffset;
        // float                 maxInterpolationOffset;
        // outScore += deviceLimits.subPixelInterpolationOffsetBits;
        outScore += deviceLimits.maxFramebufferWidth;
        outScore += deviceLimits.maxFramebufferHeight;
        outScore += deviceLimits.maxFramebufferLayers;
        // VkSampleCountFlags    framebufferColorSampleCounts;
        // VkSampleCountFlags    framebufferDepthSampleCounts;
        // VkSampleCountFlags    framebufferStencilSampleCounts;
        // VkSampleCountFlags    framebufferNoAttachmentsSampleCounts;
        outScore += deviceLimits.maxColorAttachments;
        // VkSampleCountFlags    sampledImageColorSampleCounts;
        // VkSampleCountFlags    sampledImageIntegerSampleCounts;
        // VkSampleCountFlags    sampledImageDepthSampleCounts;
        // VkSampleCountFlags    sampledImageStencilSampleCounts;
        // VkSampleCountFlags    storageImageSampleCounts;
        outScore += deviceLimits.maxSampleMaskWords;
        // VkBool32              timestampComputeAndGraphics;
        // float                 timestampPeriod;
        outScore += deviceLimits.maxClipDistances;
        outScore += deviceLimits.maxCullDistances;
        outScore += deviceLimits.maxCombinedClipAndCullDistances;
        // outScore += deviceLimits.discreteQueuePriorities;
        // float                 pointSizeRange[2];
        // float                 lineWidthRange[2];
        // float                 pointSizeGranularity;
        // float                 lineWidthGranularity;
        // VkBool32              strictLines;
        // VkBool32              standardSampleLocations;
        // VkDeviceSize          optimalBufferCopyOffsetAlignment;
        // VkDeviceSize          optimalBufferCopyRowPitchAlignment;
        // VkDeviceSize          nonCoherentAtomSize;
        return true;
    }

    uint32 ScoreDeviceDescriptor(
        const PhysicalDeviceDescriptor& deviceDescriptor,
        const VulkanDeviceScoringFuncs& scoringFunctionPtrs,
        uint32& score)
    {
        uint32 typeScore = 0;
        uint32 featuresScore = 0;
        uint32 limitsScore = 0;

        bool succeeded = scoringFunctionPtrs.m_DeviceFeatureScoringFunc != nullptr
            ? scoringFunctionPtrs.m_DeviceFeatureScoringFunc(deviceDescriptor.features, featuresScore)
            : ScoreDeviceFeatures(deviceDescriptor.features, featuresScore);

        if (!succeeded)
        {
            return false;
        }

        succeeded = scoringFunctionPtrs.m_DeviceTypeScoringFunc != nullptr
            ? scoringFunctionPtrs.m_DeviceTypeScoringFunc(deviceDescriptor.properties.deviceType, typeScore)
            : ScoreDeviceType(deviceDescriptor.properties.deviceType, typeScore);

        if (!succeeded)
        {
            return false;
        }

        succeeded = scoringFunctionPtrs.m_DeviceLimitsScoringFunc != nullptr
            ? scoringFunctionPtrs.m_DeviceLimitsScoringFunc(deviceDescriptor.properties.limits, limitsScore)
            : ScoreDeviceLimits(deviceDescriptor.properties.limits, limitsScore);

        if (!succeeded)
        {
            return false;
        }

        score = typeScore + featuresScore + limitsScore;
        return true;
    }

    bool TryChooseVulkanDevice(
        const PhysicalDeviceDescriptor* pDeviceDescs,
        const uint32& deviceDescsCount,
        VkPhysicalDevice* pOutSelectedDevice,
        uint32* pOutSelectedDeviceIndex,
        const VulkanDeviceScoringFuncs& scoringFunctionPtrs)
    {
        if ((pDeviceDescs == nullptr || deviceDescsCount == 0)
            || (pOutSelectedDevice == nullptr && pOutSelectedDeviceIndex == nullptr)) SAVANNA_BRANCH_UNLIKELY
        {
            return false;
        }

        if (deviceDescsCount == 1)
        {
            if (pOutSelectedDeviceIndex != nullptr)
            {
                *pOutSelectedDeviceIndex = 0;
            }
            if (pOutSelectedDevice != nullptr)
            {
                *pOutSelectedDevice = pDeviceDescs[0].physicalDevice;
            }
            return true;
        }

        uint32 index = 0;
        uint32 bestScore = 0;
        bool foundDevice = false;

        for (int i = 0; i < deviceDescsCount; ++i)
        {
            uint32 score = 0;
            if (!ScoreDeviceDescriptor(pDeviceDescs[i], scoringFunctionPtrs, score))
            {
                continue;
            }

            if (score > bestScore)
            {
                index = i;
                bestScore = score;
                foundDevice = true;
                if (pOutSelectedDevice != nullptr)
                {
                    *pOutSelectedDevice = pDeviceDescs[i].physicalDevice;
                }
            }
        }

        if (pOutSelectedDeviceIndex != nullptr)
        {
            *pOutSelectedDeviceIndex = index;
        }

        return foundDevice;
    }
} // namespace Savanna::Gfx::Vk::Utils
