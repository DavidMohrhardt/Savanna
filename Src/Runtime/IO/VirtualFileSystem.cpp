#include "VirtualFileSystem.h"

namespace Savanna::IO
{
    static std::filesystem::path FixupPath(const std::filesystem::path &path)
    {
        std::filesystem::path fixedPath = path;
        fixedPath.make_preferred();
        if (fixedPath.is_relative())
        {
            fixedPath = std::filesystem::current_path() / fixedPath;
        }
        if (fixedPath.has_filename())
        {
            fixedPath.remove_filename();
        }
        return fixedPath;
    }

    VirtualFileSystem::VirtualFileSystem()
        : m_RealRootPath(std::filesystem::current_path())
    {
        ParseFileSystem();
    }

    VirtualFileSystem::VirtualFileSystem(const std::filesystem::path &realRootPath)
        : m_RealRootPath(FixupPath(realRootPath))
    {
        ParseFileSystem();
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

        // for (const auto &entry : std::filesystem::recursive_directory_iterator(m_RealRootPath))
        // {
        //     // m_FileTypeMap[entry.path().extension()].push_back(entry.path());
        // }
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

    dynamic_array<std::string> VirtualFileSystem::GetAllFilesAtPath(
        const std::filesystem::path &relativePath, const char *filter) const
    {
        std::filesystem::path fullPath = GetFullPath(relativePath);
        if (!std::filesystem::exists(fullPath))
        {
            // SAVANNA_ERROR("The specified path does not exist!");
            throw Savanna::RuntimeErrorException("The specified path does not exist!");
        }

        dynamic_array<std::string> files;
        for (const auto &entry : std::filesystem::directory_iterator(fullPath))
        {
            if (filter != nullptr && entry.path().extension() != filter)
            {
                continue;
            }
            files.push_back(entry.path().string());
        }
        return files;
    }

} // namespace Savanna::IO
