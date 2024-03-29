#include "FileStream.h"

#include <exception>

Savanna::IO::FileStream::FileStream(const char *filePath)
    : m_FileStream(filePath, std::ios::ate | std::ios::binary)
{
    if (!m_FileStream.is_open())
    {
        Console::PrintLineSynchronous("{} failed to open.", filePath);
        throw Savanna::RuntimeErrorException("[Savanna::IO::FileStream] Failed to open file!");
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
