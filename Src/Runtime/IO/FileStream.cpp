#include "FileStream.h"

#include <exception>

savanna::io::FileStream::FileStream(const char *filePath)
    : m_FileStream(filePath, std::ios::ate | std::ios::binary)
{
    if (!m_FileStream.is_open())
    {
        Console::PrintLineSynchronous("{} failed to open.", filePath);
    }
}

savanna::io::FileStream::FileStream(const std::string &filePath)
    : FileStream(filePath.c_str())
{
}

savanna::io::FileStream::FileStream(const std::filesystem::path& filePath)
    : FileStream(filePath.string())
{
}

savanna::io::FileStream::FileStream(FileStream &&other)
    : m_FileStream(std::move(other.m_FileStream))
{
    other.m_FileStream = std::ifstream();
}

savanna::io::FileStream::~FileStream()
{
    if (m_FileStream.is_open())
    {
        m_FileStream.close();
    }
}
