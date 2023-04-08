#pragma once

#include "SavannaVk.h"

namespace Savanna::Gfx::Vk::Utils::IO
{
    using namespace Savanna::IO;

    // struct ShaderFileStream
    // {
    //     FileStream ShaderStream;
    //     FileStream ShaderMetadataStream;

    //     bool IsOpen() const
    //     {
    //         return ShaderStream.IsOpen() && ShaderMetadataStream.IsOpen();
    //     }

    //     std::vector<byte> ReadBinary() const
    //     {
    //         std::vector<byte> shaderBinary;
    //         if (IsOpen())
    //         {
    //             shaderBinary.resize(ShaderStream.GetSize());
    //             ShaderStream.Read(shaderBinary.data(), shaderBinary.size());
    //         }
    //         return shaderBinary;
    //     }

    //     IO::JsonDocument ReadMetaData() const
    //     {
    //         IO::XmlDocument shaderXml;
    //         if (IsOpen())
    //         {
    //             shaderXml.Load(ShaderMetadataStream);
    //         }
    //         return shaderXml;
    //     }
    // };

    struct ShaderSourceInfo
    {
        byte* m_pShaderBinary;
        ShaderMetaDataInfo* m_pShaderMetaData;
    };

    inline ShaderSourceInfo OpenShaderSourceFile(
        const std::string& shaderPath, const std::string& shaderName)
    {
        std::string shaderMetaDataName = shaderName + ".meta";
        // ShaderFileStream shaderFileStream
        // {
        //     IO::FileStream shaderStream = IO::FileStream(shaderPath, shaderName);
        //     IO::FileStream shaderMetadataStream = IO::FileStream(shaderPath, shaderMetaDataName);
        // }
        return false;
    }

} // namespace Savanna::Gfx::Vk::Utils

