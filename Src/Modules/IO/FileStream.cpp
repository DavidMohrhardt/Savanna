#include "FileStream.h"

#include <exception>

Savanna::IO::FileStream::FileStream(const char *filePath)
    : m_FileStream(filePath, std::ios::ate | std::ios::binary)
{
    if (!m_FileStream.is_open())
    {
        throw Savanna::RuntimeErrorException("Failed to open file!");
    }
}

Savanna::IO::FileStream::FileStream(const std::string &filePath)
    : FileStream(filePath.c_str())
{
}

Savanna::IO::FileStream::FileStream(const std::filesystem::path& filePath)
    : FileStream(filePath.string())
{
}

Savanna::IO::FileStream::FileStream(FileStream &&other)
    : m_FileStream(std::move(other.m_FileStream))
{
    other.m_FileStream = std::ifstream();
}

Savanna::IO::FileStream::~FileStream()
{
    if (m_FileStream.is_open())
    {
        m_FileStream.close();
    }
}

std::vector<char> Savanna::IO::FileStream::ReadFile()
{
    if (!m_FileStream.is_open())
    {
        throw Savanna::RuntimeErrorException("File is not open!");
    }

    std::vector<char> buffer(m_FileStream.seekg(0, std::ios::end).tellg());
    m_FileStream.seekg(0, std::ios::beg);
    m_FileStream.read(buffer.data(), buffer.size());

    return buffer;
}
