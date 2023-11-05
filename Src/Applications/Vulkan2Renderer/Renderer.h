#include <SavannaEngine.h>
#include <Gfx/Public/ISavannaGfx.h>

namespace vk
{
    using namespace Savanna;

    class Renderer
    {
    private:
        se_GfxShaderHandle_t m_DefaultShaderHandles[2] {};

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
