#pragma once
#include <SavannaEngine.h>
#include <Gfx/Public/ISavannaGfx.h>

#include <Types/Containers/Arrays/dynamic_array.h>

namespace vk
{
    using namespace savanna;

    class Renderer
    {
    public:
        Renderer();
        ~Renderer();

        bool TryInitialize(
            const se_GfxContextCreateInfo_t* pCreateInfo,
            // TODO change this to a create info for the window
            void* pWindowHandle);

        void Render();
    };
} // namespace vk
