#include "System.h"

namespace savanna::Entities
{
    se_EntitySystemErrorCode_t System::Initialize(
        const se_EntitySystemInterface_t *pInterface) noexcept
    {
        if (pInterface == nullptr)
        {
            return kSavannaSystemMalformedInput;
        }

        if (m_pInterface != nullptr)
        {
            return kSavannaSystemAlreadyInitialized;
        }

        m_pInterface = const_cast<se_EntitySystemInterface_t*>(pInterface);

        return kSavannaSystemSuccess;
    }
} // namespace savanna::Entities

