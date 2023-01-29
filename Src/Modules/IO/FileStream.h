/**
 * @file FileStream.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief
 * @version 0.1
 * @date 2023-01-27
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <SavannaEngine.h>
#include <Utilities/SavannaCoding.h>

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace Savanna::IO
{
    class FileStream : public NonCopyable
    {
    private:
        std::ifstream m_FileStream;

    public:
        FileStream() = default;
        FileStream(const char* filePath);
        FileStream(const std::string& filePath);
        FileStream(const std::filesystem::path& filePath);
        FileStream(const FileStream& other) = delete;
        FileStream(FileStream&& other);
        ~FileStream();

    public:
        std::vector<char> ReadFile();

    public:
        inline bool IsOpen() const { return m_FileStream.is_open(); }
    };
} // namespace Savanna::IO
