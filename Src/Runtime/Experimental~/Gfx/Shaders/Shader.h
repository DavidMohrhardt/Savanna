#include <SavannaEngine.h>

typedef uint64_t se_ShaderHandle_t;
SAVANNA_CONSTEXPR se_ShaderHandle_t k_InvalidShaderHandle = 0;

#if __cplusplus
namespace Savanna::Gfx
{
    struct ShaderHandle
    {
        ShaderHandle() : m_Handle(k_InvalidShaderHandle) {}
        ShaderHandle(se_ShaderHandle_t handle) : m_Handle(handle) {}
        ShaderHandle(const ShaderHandle& other) : m_Handle(other.m_Handle) {}
        ShaderHandle(ShaderHandle&& other) : m_Handle(other.m_Handle) { other.m_Handle = k_InvalidShaderHandle; }

        ShaderHandle& operator=(const ShaderHandle& other)
        {
            m_Handle = other.m_Handle;
            return *this;
        }

        ShaderHandle& operator=(ShaderHandle&& other)
        {
            m_Handle = other.m_Handle;
            other.m_Handle = k_InvalidShaderHandle;
            return *this;
        }

        bool operator==(const ShaderHandle& other) const { return m_Handle == other.m_Handle; }
        bool operator!=(const ShaderHandle& other) const { return m_Handle != other.m_Handle; }

        operator se_ShaderHandle_t() const { return m_Handle; }

        se_ShaderHandle_t handle;
    };

    class ShaderMap
    {
    private:
        std::unordered_map<se_ShaderHandle_t, Shader> m_ShaderMap;
    };
} // namespace Savanna::Gfx
#endif
