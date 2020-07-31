#pragma once

#include "Core/Type.h"

#include "RuntimeReflection/Macro.h"

namespace SteelEngine { namespace Graphics {

    SE_STRUCT()
    struct VertexBuffer
    {
        virtual ~VertexBuffer() { }

        SE_METHOD()
        static VertexBuffer* Create(float* vertices, Type::uint32 count);

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual Type::uint32 GetID() const = 0;
    };

}}