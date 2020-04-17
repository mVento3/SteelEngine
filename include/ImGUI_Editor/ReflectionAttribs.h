#pragma once

namespace SteelEngine { namespace Editor {

    enum ReflectionAttributes
    {
        EDITOR_WINDOW,
        RANGE,
    // There will be two options: STATIC and CONTEXT / STATIC mean there will be only one instance of window and CONTEXT mean there will be that much as needed
        WINDOW_TYPE
    };

    struct Range
    {
        Range(float min, float max) :
            m_Min(min),
            m_Max(max)
        {
            
        }

        float m_Min;
        float m_Max;
    };

}}