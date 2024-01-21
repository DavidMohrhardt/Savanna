#include "System.h"

namespace savanna::Entities
{
    seEntitySystemErrorCode System::Initialize(
        const seEntitySystemInterface *pInterface) noexcept
    {
        if (pInterface == nullptr)
        {
            return kSavannaSystemMalformedInput;
        }

        if (m_pInterface != nullptr)
        {
            return kSavannaSystemAlreadyInitialized;
        }

        m_pInterface = const_cast<seEntitySystemInterface*>(pInterface);

        return kSavannaSystemSuccess;
    }
} // namespace savanna::Entities

