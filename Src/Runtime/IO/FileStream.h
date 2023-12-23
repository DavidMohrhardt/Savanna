/**
 * @file FileStream.h
 * @author David Mohrhardt (https://github.com/DavidMohrhardt/Savanna)
 * @brief TODO @David.Mohrhardt Document
 * @version 0.1
 * @date 2023-01-27
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "SavannaEngine.h"
#include "Utilities/SavannaCoding.h"
#include "Types/Containers/Arrays/dynamic_array.h"

#include <filesystem>
#include <fstream>
#include <string>

namespace savanna::io
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
        template<typename T>
        dynamic_array<T> ReadFile(const AllocatorKind allocatorKind = kSavannaAllocatorKindHeap)
        {
            dynamic_array<T> buffer(0, allocatorKind);
            if (!m_FileStream.is_open())
            {
                return buffer;
            }

            m_FileStream.seekg(0, std::ios::beg);
            m_FileStream.seekg(0, std::ios::end);
            buffer.resize(m_FileStream.tellg() / sizeof(T));
            m_FileStream.seekg(0, std::ios::beg);
            m_FileStream.read(reinterpret_cast<char*>(buffer.data()), buffer.size() * sizeof(T));

            return buffer;
        }

    public:
        inline bool IsOpen() const { return m_FileStream.is_open(); }
    };
} // namespace savanna::IO
