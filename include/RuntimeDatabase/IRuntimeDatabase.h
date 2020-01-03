#pragma once

namespace SteelEngine {

    struct IRuntimeDatabase
    {
        virtual size_t GetNextPerVariantID() = 0;
        virtual void PushPerVariantID(size_t id) = 0;
        virtual size_t GetNextPerObjectID() = 0;
        virtual void PushPerObjectID(size_t id) = 0;
    };

}