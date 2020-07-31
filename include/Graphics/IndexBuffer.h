#pragma once

#include "Core/Type.h"

#include "RuntimeReflection/Macro.h"

namespace SteelEngine { namespace Graphics {

    SE_STRUCT()
    struct IndexBuffer
    {
        virtual ~IndexBuffer() { }

        SE_METHOD()
        static IndexBuffer* Create(Type::uint32* indices, Type::uint32 count);

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual Type::uint32 GetCount() const = 0;
        virtual Type::uint32 GetID() const = 0;
    };

}}