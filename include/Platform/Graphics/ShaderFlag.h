#pragma once

namespace SteelEngine { namespace Graphics {

    enum class ShaderFlag
    {
        COMPILE_STATUS,
        LINK_STATUS,
        VALIDATE_STATUS
    };

    enum class ShaderType
    {
        VERTEX_SHADER,
        FRAGMENT_SHADER,
        GEOMETRY_SHADER
    };

}}