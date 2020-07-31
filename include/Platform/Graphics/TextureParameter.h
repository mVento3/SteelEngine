#pragma once

namespace SteelEngine { namespace Graphics {

    enum class ParameterName
    {
        TEXTURE_WRAP_S,
        TEXTURE_WRAP_T,
        TEXTURE_MIN_FILTER,
        TEXTURE_MAG_FILTER
    };

    enum class ParameterValue
    {
        REPEAT,
        CLAMP_TO_EDGE
    };

    enum class Filter
    {
        NEAREST,
        LINEAR
    };

    enum class Format
    {
        RGBA,
        RGB,
        RED,
        GREEN,
        BLUE,
        RG,
        BGR,
        BGRA
    };

    enum class InternalFormat
    {
        RGBA,
        RGB,
        RGB16F
    };

    enum class DataType
    {
        UNSIGNED_BYTE,
        UNSIGNED_SHORT,
        UNSIGNED_INT,
        BYTE,
        SHORT,
        INT,
        FLOAT
    };

}}