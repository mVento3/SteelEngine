#pragma once

namespace SteelEngine { namespace VisualScript {

    struct IPinData
    {
        virtual void* GetRawData() const = 0;
        virtual void SetRawData(void* data) = 0;
        virtual size_t GetValueTypeID() const = 0;

        template <typename A>
        A GetData()
        {
            A* dataPtr = (A*)GetRawData();

            return *dataPtr;
        }

        template <typename A>
        void SetData(const A& data)
        {
            A* dataPtr = (A*)GetRawData();

            *dataPtr = data;
        }
    };

}}