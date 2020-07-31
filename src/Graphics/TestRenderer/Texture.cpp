#include "Graphics/TestRenderer/Texture.h"

#include "stbi/stb_image.h"

namespace SteelEngine { namespace Graphics { namespace TestRenderer {

    Texture::Texture()
    {
        m_ID = NULL;
        m_Data = 0;
    }

    Texture::Texture(const char* filename)
    {
        m_Data = stbi_load(filename, &m_Width, &m_Height, &m_Channels, 4);

        m_Filter = Filter::LINEAR;
        m_InternalFormat = InternalFormat::RGBA;
        m_Format = Format::RGBA;
        m_Type = DataType::UNSIGNED_BYTE;
        m_Clamp = false;
        m_ID = NULL;
    }

    Texture::Texture(
        InternalFormat internalFormat,
        Format format,
        DataType type,
        Filter filter,
        bool clamp) :
        m_InternalFormat(internalFormat),
        m_Format(format),
        m_Type(type),
        m_Filter(filter),
        m_Clamp(clamp)
    {
        m_Data = 0;
        m_ID = NULL;
    }

    Texture::Texture(
        uint32_t width,
        uint32_t height,
        InternalFormat internalFormat,
        Format format,
        DataType type,
        Filter filter,
        bool clamp) :
        Texture(internalFormat, format, type, filter, clamp)
    {
        m_Width = width;
        m_Height = height;
    }

    Texture::~Texture()
    {

    }

    void Texture::Setup(const RenderDevice* renderDevice)
    {
        renderDevice->GenerateTexture(this);
        renderDevice->Bind(this);

        if(m_Clamp)
        {
            renderDevice->TextureParameter(ParameterName::TEXTURE_WRAP_S, ParameterValue::REPEAT);
            renderDevice->TextureParameter(ParameterName::TEXTURE_WRAP_T, ParameterValue::REPEAT);
        }
        else
        {
            renderDevice->TextureParameter(ParameterName::TEXTURE_WRAP_S, ParameterValue::CLAMP_TO_EDGE);
            renderDevice->TextureParameter(ParameterName::TEXTURE_WRAP_T, ParameterValue::CLAMP_TO_EDGE);
        }

        renderDevice->TextureParameter(ParameterName::TEXTURE_MIN_FILTER, m_Filter);
        renderDevice->TextureParameter(ParameterName::TEXTURE_MAG_FILTER, m_Filter);

        renderDevice->TextureImage2D(this, m_InternalFormat, m_Width, m_Height, m_Format, m_Type, m_Data);

        if(m_Data)
        {
            stbi_image_free(m_Data);
        }
    }

    void Texture::SetWidth(uint32_t width)
    {
        m_Width = width;
    }

    void Texture::SetHeight(uint32_t height)
    {
        m_Height = height;
    }

    Platform::Graphics::ITexture** Texture::GetTextureID_Ptr() const
    {
        return (Platform::Graphics::ITexture**)&m_ID;
    }

    unsigned int Texture::GetTextureWidth() const
    {
        return m_Width;
    }

    unsigned int Texture::GetTextureHeight() const
    {
        return m_Height;
    }

}}}