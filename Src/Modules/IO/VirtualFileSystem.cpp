#include "VirtualFileSystem.h"

namespace Savanna::IO
{
    VirtualFileSystem::VirtualFileSystem()
    {
    }

    VirtualFileSystem::VirtualFileSystem(const std::filesystem::path &realRootPath)
    {
    }

    VirtualFileSystem::~VirtualFileSystem()
    {
    }

    void VirtualFileSystem::ParseFileSystem()
    {
        if (!std::filesystem::exists(m_RealRootPath))
        {
            // SAVANNA_ERROR("The specified root path does not exist!");
            throw Savanna::RuntimeErrorException("The specified root path does not exist!");
        }

        for (const auto &entry : std::filesystem::recursive_directory_iterator(m_RealRootPath))
        {
            // m_FileTypeMap[entry.path().extension()].push_back(entry.path());
        }
    }

} // namespace Savanna::IO
