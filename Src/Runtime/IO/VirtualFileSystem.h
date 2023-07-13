/**
 * @file FileSystem.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-01-05
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "SavannaEngine.h"
#include "Utilities/SavannaCoding.h"

#include "Types/Singleton/Singleton.h"

#include <filesystem>
#include <unordered_map>

namespace Savanna::IO
{
    class VirtualFileSystem : public Singleton<VirtualFileSystem>
    {
    private:
        friend class Singleton<VirtualFileSystem>;

        std::filesystem::path m_RealRootPath;
        std::unordered_map<std::filesystem::file_type, std::vector<std::filesystem::path>> m_FileTypeMap;

    private:
        VirtualFileSystem();
        VirtualFileSystem(const std::filesystem::path &realRootPath);
        VirtualFileSystem(const VirtualFileSystem &other) = delete;

    public:
        ~VirtualFileSystem();

        void ParseFileSystem();

        std::filesystem::path GetFullPath(const std::filesystem::path &relativePath) const;

        std::string GetFileNameWithExtension(const std::filesystem::path &path) const;

        std::string GetFileNameWithoutExtension(const std::filesystem::path &path) const;
    };
} // namespace Savanna::IO
