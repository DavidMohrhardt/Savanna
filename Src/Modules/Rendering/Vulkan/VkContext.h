#pragma once

#include <SavannaEngine.h>
#include <Types/Strings/FixedString.h>

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include <vulkan/vulkan.h>

#include "VkRendererCreateInfo.h"

#include "Utilities/VkCallbacks.h"
#include "Utilities/VkDefinitions.h"
#include "Utilities/VkMacros.h"
#include "Utilities/VkResultUtils.h"

namespace Savanna::Gfx::Vk
{
    // Forward Declarations
    class DebugMessenger;

    class Context
    {
    private: // Members
        VkInstance m_Instance;
        std::unique_ptr<DebugMessenger> m_DebugMessenger;

    private: // Private Constructors

    public: // Constructors/Destructors
        Context(const RendererCreateInfo* const pCreateInfo);

        Context();
        Context(const Context &other) = delete;
        Context(Context &&other);

        ~Context();

    public: // Operators
        operator VkInstance() const { return m_Instance; }

        Context& operator=(const Context &other) = delete;
        Context& operator=(Context &&other);

    public: // Functions
        inline VkInstance GetVkInstance() const { return m_Instance; }
        inline bool IsValid() const { return m_Instance != VK_NULL_HANDLE; }

    private: // Functions

        void CreateVkInstance(VkInstanceCreateInfo* pInstanceCreateInfo);
        void CreateDebugMessenger(const RendererCreateInfo* const pRendererCreateInfo);

        void PopulateAdditionalInstanceInfo(VkInstanceCreateInfo* pInstanceCreateInfo);
    };
} // namespace Savanna::Gfx::Vk
