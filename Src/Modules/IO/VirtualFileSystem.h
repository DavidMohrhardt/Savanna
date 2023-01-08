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

#include <SavannaEngine.h>
#include <Utilities/SavannaCoding.h>

#include <filesystem>
#include <unordered_map>

namespace Savanna::IO
{
    class VirtualFileSystem
    {
    private:
        std::filesystem::path m_RealRootPath;
        std::unordered_map<std::filesystem::file_type, std::vector<std::filesystem::path>> m_FileTypeMap;

    public:
        VirtualFileSystem();
        VirtualFileSystem(const std::filesystem::path &realRootPath);
        VirtualFileSystem(const VirtualFileSystem &other) = delete;
        VirtualFileSystem(VirtualFileSystem &&other) SAVANNA_NO_EXCEPT;
        ~VirtualFileSystem();

    public:
        void ParseFileSystem();
    };
} // namespace Savanna::IO
