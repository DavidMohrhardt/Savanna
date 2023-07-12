#include "VirtualFileSystem.h"

namespace Savanna::IO
{
    VirtualFileSystem::VirtualFileSystem()
    {
        // Get the current working directory
        m_RealRootPath = std::filesystem::current_path();
    }

    VirtualFileSystem::VirtualFileSystem(const std::filesystem::path &realRootPath)
        : m_RealRootPath(realRootPath)
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

    std::filesystem::path VirtualFileSystem::GetFullPath(const std::filesystem::path &relativePath) const
    {
        return m_RealRootPath / relativePath;
    }

    std::string VirtualFileSystem::GetFileNameWithExtension(const std::filesystem::path &path) const
    {
        return path.filename().string();
    }

    std::string VirtualFileSystem::GetFileNameWithoutExtension(
        const std::filesystem::path &path) const
    {
        return path.stem().string();
    }

} // namespace Savanna::IO
